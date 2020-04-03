#include <string>
#include <list>
#include "test_runner.h"
using namespace std;

class Editor {
public:
    // Реализуйте конструктор по умолчанию и объявленные методы
    Editor() {
        _cursor = _text.begin();
    }
    void Left() {
        if( _cursor == _text.begin() ) return;
        _cursor--;
    }
    void Right() {
        if( _cursor == _text.end() ) return;
        _cursor++;
    }
    void Insert(char token) {
        _text.insert(_cursor, token);
    }
    void Cut(size_t tokens = 1) {
        if( tokens == 0) {
            _buffer = "";
            return;
        }

        auto last = _cursor;
        for (last; last != _text.end() && tokens>0; tokens--) {
            last = next(last);
        }

        _buffer = string(_cursor, last);
        _cursor = _text.erase(_cursor, last);
    }
    void Copy(size_t tokens = 1) {
        if( tokens = 0) {
            _buffer = "";
            return;
        }
        string buf(_cursor, next(_cursor, tokens));
        _buffer = buf;
    }
    void Paste() {
        for( char c : _buffer ) {
            Editor::Insert(c);
        }
    }
    string GetText() const {
        return string(_text.begin(), _text.end());
    }
    
private:
    list<char> _text;
    list<char>::iterator _cursor;
    string _buffer;
};

void TypeText(Editor& editor, const string& text) {
    for (char c : text) {
        editor.Insert(c);
    }
}

void TestEditing() {
    {
        Editor editor;

        const size_t text_len = 12;
        const size_t first_part_len = 7;
        TypeText(editor, "hello, world");
        for (size_t i = 0; i < text_len; ++i) {
            editor.Left();
        }
        editor.Cut(first_part_len);
        cout << editor.GetText() << endl;
        for (size_t i = 0; i < text_len - first_part_len; ++i) {
            editor.Right();
        }
        TypeText(editor, ", ");

        editor.Paste();
        editor.Left();
        editor.Left();
        editor.Cut(3);

        ASSERT_EQUAL(editor.GetText(), "world, hello");
    }
    {
        Editor editor;

        TypeText(editor, "misprnit");
        editor.Left();
        editor.Left();
        editor.Left();
        editor.Cut(1);
        editor.Right();
        editor.Paste();

        ASSERT_EQUAL(editor.GetText(), "misprint");
    }
}

void TestReverse() {
    Editor editor;

    const string text = "esreveR";
    for (char c : text) {
        editor.Insert(c);
        editor.Left();
    }

    ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
    Editor editor;
    ASSERT_EQUAL(editor.GetText(), "");

    editor.Left();
    editor.Left();
    editor.Right();
    editor.Right();
    editor.Copy(0);
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
    Editor editor;

    editor.Paste();
    TypeText(editor, "example");
    editor.Left();
    editor.Left();
    editor.Paste();
    editor.Right();
    editor.Paste();
    editor.Copy(0);
    editor.Paste();
    editor.Left();
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
    
    TestRunner tr;
    RUN_TEST(tr, TestEditing);
    RUN_TEST(tr, TestReverse);
    RUN_TEST(tr, TestNoText);
    RUN_TEST(tr, TestEmptyBuffer);
    
    /*list<char> words1{'t', 'f', 'i', 'e', 'd'};
    auto c = words1.begin();
    c++;
    c++;
    c--;
    words1.insert(c, 's');
    words1.insert(c, 'n');
    string s(c, next(c, 2));
    //cout << s;
    
    words1.insert(c, 'a');
    
    words1.erase(prev(c, 4), c);
    for( auto w : words1 ) {
        cout << w << " ";
    }*/
    
    return 0;
}