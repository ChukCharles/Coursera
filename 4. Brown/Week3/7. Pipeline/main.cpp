#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;


struct Email {
    string from;
    string to;
    string body;
};


class Worker {
public:
    virtual ~Worker() = default;
    virtual void Process(unique_ptr<Email> email) = 0;
    virtual void Run() {
        // только первому worker-у в пайплайне нужно это имплементировать
        throw logic_error("Unimplemented");
    }

protected:
    // реализации должны вызывать PassOn, чтобы передать объект дальше
    // по цепочке обработчиков
    void PassOn(unique_ptr<Email> email) const {
        if ( next_ ) next_->Process(move(email));
    }

public:
    void SetNext(unique_ptr<Worker> next) {
        next_ = move(next);
    }
private:
    unique_ptr<Worker> next_;
};


class Reader : public Worker {
public:
    // реализуйте класс
    Reader(istream& in) : input(in)
    {}

    void Process(unique_ptr<Email> email) {
        PassOn(move(email));
    }

    void Run() override {
        string from_s, to, body, empty;
        while (getline(input, from_s) && getline(input, to) && getline(input, body)) {
            unique_ptr<Email> item_ptr = make_unique<Email>(Email({ from_s, to, body }));
            Process(move(item_ptr));
        }
    }

private:
    istream& input;
};


class Filter : public Worker {
public:
    using Function = function<bool(const Email&)>;
    Filter(Function filter) : filter_(filter) {}

public:
    void Process(unique_ptr<Email> email) override  {
        if (filter_(*email.get())) PassOn(move(email));
    }

private:
    Function filter_;
};


class Copier : public Worker {
public:
    // реализуйте класс
    Copier(const string& adr) : adress_(adr)
    {}

    void Process(unique_ptr<Email> email) override {
        Email mail_copy{ email.get()->from, adress_, email.get()->body };
        string to_copy = email.get()->to;
        PassOn(move(email));
        if (to_copy != adress_) {
            PassOn(make_unique<Email>(mail_copy));
        }
    }
private:
    string adress_;
};


class Sender : public Worker {
public:
    // реализуйте класс
    Sender(ostream& out) : output(out)
    {}

    void Process(unique_ptr<Email> email) override {
        output << email->from << "\n" 
            << email->to << "\n" 
            << email->body << "\n";
        PassOn(move(email));
    }
private:
    ostream& output;
};


// реализуйте класс
class PipelineBuilder {
public:
    // добавляет в качестве первого обработчика Reader
    explicit PipelineBuilder(istream& in) {
        //first_ = make_unique<Reader>(in);
        pipeline_.push_back(make_unique<Reader>(in));
    }

    // добавляет новый обработчик Filter
    PipelineBuilder& FilterBy(Filter::Function filter) {
        pipeline_.push_back(make_unique<Filter>(filter));
        return *this;
    }

    // добавляет новый обработчик Copier
    PipelineBuilder& CopyTo(string recipient) {
        pipeline_.push_back(make_unique<Copier>(recipient));
        return *this;
    }

    // добавляет новый обработчик Sender
    PipelineBuilder& Send(ostream& out) {
        pipeline_.push_back(make_unique<Sender>(out));
        return *this;
    }

    // возвращает готовую цепочку обработчиков
    unique_ptr<Worker> Build() {
        //return move(*pipeline_.begin());
        unique_ptr<Worker> next = move(pipeline_.back());
        pipeline_.pop_back();
        while (pipeline_.size() > 0) {
            pipeline_.back()->SetNext(move(next));
            next = move(pipeline_.back());
            pipeline_.pop_back();
        }
        return move(next);
    }
private:
    unique_ptr<Worker> first_;
    vector<unique_ptr<Worker>> pipeline_;
};


void TestSanity() {
    string input = (
        "erich@example.com\n"
        "richard@example.com\n"
        "Hello there\n"

        "erich@example.com\n"
        "ralph@example.com\n"
        "Are you sure you pressed the right button?\n"

        "ralph@example.com\n"
        "erich@example.com\n"
        "I do not make mistakes of that kind\n"
        );
    istringstream inStream(input);
    ostringstream outStream;

    PipelineBuilder builder(inStream);
    builder.FilterBy([](const Email& email) {
        return email.from == "erich@example.com";
        });
    builder.CopyTo("richard@example.com");
    builder.Send(outStream);
    auto pipeline = builder.Build();

    pipeline->Run();

    string expectedOutput = (
        "erich@example.com\n"
        "richard@example.com\n"
        "Hello there\n"

        "erich@example.com\n"
        "ralph@example.com\n"
        "Are you sure you pressed the right button?\n"

        "erich@example.com\n"
        "richard@example.com\n"
        "Are you sure you pressed the right button?\n"
        );

    ASSERT_EQUAL(expectedOutput, outStream.str());
    //cout << outStream.str() << endl;
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSanity);
    return 0;
}
