#include "test_runner.h"
#include <string>
#include <queue>
#include <algorithm>
#include <map>

using namespace std;

class Translator {
public:

    void Add(string_view source, string_view target) {
        string s(source), t(target);
        auto search = find(_source.begin(), _source.end(), source );
        if (search != _source.end()) {
            auto index = search - _source.begin();
            _target[index] = target;
        } else {
            _source.push_back(s);
            _target.push_back(t);
        }
    }

    string_view TranslateForward(string_view source) const {
        auto r = find(_source.begin(), _source.end(), source);
        if (r != _source.end()) {
            int idx = r - _source.begin();
            return _target[idx];
        } else {
            return string_view("");
        }
    }

    string_view TranslateBackward(string_view target) const {
        auto r = find(_target.begin(), _target.end(), target);
        if (r != _target.end()) {
            int idx = r - _target.begin();
            return _source[idx];
        } else {
            return string_view("");
        }
    }

private:
    vector<string> _source;
    vector<string> _target;
};

void TestSimple() {
    Translator translator;
    translator.Add(string("okno"), string("window"));
    translator.Add(string("stol"), string("table"));

    ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
    ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
    ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSimple);

    return 0;
}
