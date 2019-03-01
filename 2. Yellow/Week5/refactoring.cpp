#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

class Person {
public:
    Person( const string& s ) : Name_(s) {}

    virtual string Profession() const = 0;
    string Name() const { return Name_; }
    virtual void Walk( const string& destination ) const {
        cout << Profession() << ": " << Name_ << " walks to: " << destination << endl;
    }

private:
    string Name_;
};

class Student : public Person {
public:
    Student(const string& name, const string& favouriteSong) 
        : Person(name),  FavoriteSong_(favouriteSong) 
    {}

    string Profession() const override {
        return "Student";
    }

    void Walk(const string& destination) const override {
        cout << "Student walks to: " << destination << endl;
        SingSong();
    }

    void Learn() const {
        cout << "Student: " << Name() << " learns" << endl;
    }

    void SingSong() const {
        cout << "Student: " << Name() << " sings a song: " << FavoriteSong_ << endl;
    }

private:
    string FavoriteSong_;
};


class Teacher : public Person {
public:
    Teacher(const string& name, string subject)
        : Person(name), Subject_(subject) 
    {}

    string Profession() const override {
        return "Teacher";
    }

    void Teach() const {
        cout << "Teacher: " << Name() << " teaches: " << Subject_ << endl;
    }

private:
    string Subject_;
};

class Policeman : public Person {
public:
    Policeman(const string& name) : Person(name) {}
    
    virtual string Profession() const override { return "Policeman"; }

    void Check(const shared_ptr<Person> p) const {
        cout << "Policeman: " << Name() << " checks " << p->Profession() << ". " 
                << p->Profession() << "'s name is: " << p->Name() << endl;
    }
};

void VisitPlaces(shared_ptr<Person> person, vector<string> places) {
    for (auto p : places) {
        person->Walk(p);
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

    Policeman p("Bob");
    p.Walk("Work");

    VisitPlaces(suspiciosPersons[0], {"Bagdad", "Kair", "Deli"});
    VisitPlaces(suspiciosPersons[1], {"Bagdad", "Astana"});
    VisitPlaces(suspiciosPersons[2], {"Rio", "New York"});
    VisitPlaces(suspiciosPersons[3], {"Paris"});
    VisitPlaces(suspiciosPersons[4], {"Deli", "Astana"});

    p.Check(suspiciosPersons[1]);
    p.Check(suspiciosPersons[4]);
    
    return 0;
}
