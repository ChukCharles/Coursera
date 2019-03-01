#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

class Person {
public:
    Person( const string& n, const string& p ) : Name_(n), Profession_(p) {}

    string Name() const { return Name_; }
    string Profession() const { return Profession_; }
    string LogBase() const {
        return string( Profession() + ": " + Name());
    }
    virtual void Walk( const string& destination ) const {
        cout << LogBase() << " walks to: " << destination << endl;
    }

private:
    const string Name_;
    const string Profession_;
};

class Student : public Person {
public:
    Student(const string& name, const string& favouriteSong) 
        : Person(name, "Student"),  FavoriteSong_(favouriteSong) 
    {}

    void Walk(const string& destination) const override {
        Person::Walk(destination);
        SingSong();
    }

    void Learn() const {
        cout << LogBase() << " learns" << endl;
    }

    void SingSong() const {
        cout << LogBase() << " sings a song: " << FavoriteSong_ << endl;
    }

private:
    const string FavoriteSong_;
};


class Teacher : public Person {
public:
    Teacher(const string& name, string subj)
        : Person(name, "Teacher"), Subject_(subj) 
    {}

    void Teach() const {
        cout << LogBase() << " teaches: " << Subject_ << endl;
    }

private:
    const string Subject_;
};

class Policeman : public Person {
public:
    Policeman(const string& name) : Person(name,  "Policeman") {}

    void Check(const Person& p) const {
        cout << LogBase() << " checks " << p.Profession() << ". " 
                << p.Profession() << "'s name is: " << p.Name() << endl;
    }
};

void VisitPlaces( Person& person, vector<string> places) {
    for (auto p : places) {
        person.Walk(p);
    }
}

int main() {
    vector<shared_ptr<Person>> suspiciosPersons;
    suspiciosPersons.push_back( make_shared<Teacher>( "Jim", "Chemistry" ));
    suspiciosPersons.push_back( make_shared<Teacher>( "Jim", "Physics" ));
    suspiciosPersons.push_back( make_shared<Student>("Ann", "We will rock you"));
    suspiciosPersons.push_back( make_shared<Student>("Mike", "Smoke on the water"));
    suspiciosPersons.push_back( make_shared<Student>("Threvor", "Seven Nation Army"));
    suspiciosPersons.push_back( make_shared<Policeman>("Kelly"));
    
    for( auto p : suspiciosPersons ) {
        VisitPlaces( *p, {"Morocco", "Spain"});
    }

    Policeman p("Bob");
    p.Walk("Work");

    Student s1("Mike", "Smoke on the water");
    Teacher t1( "Jim", "Physics" );
    

    p.Check(s1);
    p.Check(t1);
    
    return 0;
}
