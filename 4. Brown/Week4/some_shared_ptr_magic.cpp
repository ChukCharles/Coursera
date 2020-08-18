#include "Common.h"
#include "test_runner.h"

#include <atomic>
#include <future>
#include <numeric>
#include <random>
#include <sstream>

using namespace std;

class Book {
public:
    Book( string name ) : name_(move(name))
    {
        cout << name_ << " c-tor" << endl;
    }
    Book(Book& source) : name_(source.name_)
    {
        cout << name_ << " copy c-tor" << endl;
    }
    ~Book() {
        cout << name_ << " d-tor" << endl;
    }

    const string& GetName() const {
        return name_;
    }

private:
    string name_;
};



int main() {
    Book b1("book1");
    shared_ptr<Book> ptr1 = make_shared<Book>(b1);
    cout << ptr1->GetName() << endl;
    {
        shared_ptr<Book> ptr2 = make_shared<Book>(b1);
        cout << ptr2->GetName() << endl;
    }
    cout << ptr1->GetName() << endl;

    Book* raw_ptr = new Book("book2");
    shared_ptr<Book> ptr3(raw_ptr);
    cout << ptr3->GetName() << endl;
    {
        shared_ptr<Book> ptr4(raw_ptr);
        cout << ptr4->GetName() << endl;
    }
    //book2 was deleted earlier ^
    cout << ptr3->GetName() << endl;
    return 0;
}
