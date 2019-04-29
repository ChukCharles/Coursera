#include "test_runner.h"
#include "profile.h"

#include <iostream>
#include <list>

using namespace std;

class Editor {
public:
    // Реализуйте конструктор по умолчанию и объявленные методы
    Editor() {
        cursor = data.begin();
    }
    void Left() {
        if( cursor != data.begin() ) {
            --cursor;
        }
    }
    void Right() {
        if( cursor != data.end() ) {
            ++cursor;
        }
    }
    void Insert(char token) {
        cursor = data.insert( cursor, token );
        cursor++;
    }
    void Cut(size_t tokens = 1) {
        buffer.clear();
        list<char>::iterator last = cursor;
        for( int i = 0; i < tokens && last != data.end(); ++i ) {
            buffer.push_back(*last);
            ++last;
        }
        cursor = data.erase( cursor, last );
    }
    
    void Copy(size_t tokens = 1) {
        buffer.clear();
        list<char>::iterator tmp = cursor;
        for( int i = 0; i < tokens && tmp != data.end(); ++i ) {
            buffer.push_back(*tmp);
            ++tmp;
        }
    }
    void Paste() {
        if( buffer.size() != 0 ) {
            cursor = data.insert( cursor, buffer.begin(), buffer.end() );
            for(int i = 0; i < buffer.size(); ++i) {
                ++cursor;
            }
        }        
    }
    string GetText() const {
        string res;
        for( const char& c : data ) {
            res += c;
        }
        return res;
    }
    
    void PrintBuffer() const {
        for( const char& c : buffer ) {
            cout << c;
        }
        cout << endl;
    }
    
    void PrintAfterCursor() const {
        auto tmp = cursor;
        while( tmp != data.end() ) {
            cout << *tmp;
            ++tmp;
        }
    }

private:
    list<char> data;
    list<char> buffer;
    list<char>::iterator cursor;
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

    /*Editor editor;
    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for (size_t i = 0; i < text_len; ++i) {
        editor.Left();
    }
    editor.PrintAfterCursor();
    editor.Cut(first_part_len);
    cout << editor.GetText();*/
    
    return 0;
}