#include "test_runner.h"
#include <string>
#include <string_view>
#include <map>
#include <deque>

using namespace std;

string empty_str = "";

class Translator {
public:
    Translator() {};
    
    void Add(string_view source, string_view target)
    {
        string_view src, trgt;
        auto src_it = forward.find(source);
        if( src_it != forward.end() ) {
            src = src_it->first;
        } else {
            src = source_dict.emplace_back(string(source));
        }
        
        auto trgt_it = backward.find(target);
        if( trgt_it != backward.end() ) {
            trgt = trgt_it->first;
        } else {
            trgt = target_dict.emplace_back(target);
        }

        forward[src] = trgt;
        backward[trgt] = src;
    }
    string_view TranslateForward(string_view source) const {
        if( forward.count(source) < 1 ) {
            return string_view(empty_str);
        }
        return forward.at(source);
    }
    string_view TranslateBackward(string_view target) const {
        if( backward.count(target) < 1 ) {
            return string_view(empty_str);
        }
        return backward.at(target);
    }

private:
    deque<string> source_dict;
    deque<string> target_dict;
    map<string_view, string_view> forward;
    map<string_view, string_view> backward;
    
};

void TestSimple() {
    Translator translator;
    translator.Add(string("okno"), string("window"));
    translator.Add(string("stol"), string("table"));

    ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
    ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
    ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
    
    translator.Add(string("stol"), string("tablet"));
    ASSERT_EQUAL(translator.TranslateForward("stol"), "tablet");
    ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
    ASSERT_EQUAL(translator.TranslateBackward("tablet"), "stol");
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSimple);
    
    return 0;
}