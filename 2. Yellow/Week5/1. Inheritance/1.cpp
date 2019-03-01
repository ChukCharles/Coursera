#include <iostream>
#include <string>

using namespace std;

class Animal {
public:
    const string Name;
    Animal( const string n ) : Name(n) {}
};


class Dog : public Animal {
public:
    Dog( const string& s ) : Animal( s ) {}
    void Bark() {
        cout << Name << " barks: woof!" << endl;
    }
};
/*
int main() {
    string nj = "Jack";
    Dog d1( "Jack" );
    d1.Bark();

    return 0;
}*/