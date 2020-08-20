#include <stdexcept>
#include<vector>
#include<algorithm>
#include<iostream>

using namespace std;

enum class Gender {
    FEMALE,
    MALE
};

struct Person {
    int age;  // возраст
    Gender gender;  // пол
    bool is_employed;  // имеет ли работу
};

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
    if (range_begin == range_end) {
        return 0;
    }
    vector<typename InputIt::value_type> range_copy(range_begin, range_end);
    auto middle = begin(range_copy) + range_copy.size() / 2;
    nth_element(
        begin(range_copy), middle, end(range_copy),
        [](const Person& lhs, const Person& rhs) {
            return lhs.age < rhs.age;
        }
    );
    return middle->age;
}

void PrintStats(vector<Person> persons) {
    //all
    int all = ComputeMedianAge(persons.begin(), persons.end());
    std::cout << "Median age = " << all << std::endl;
    
    { //females
        auto it = std::partition(persons.begin(), persons.end(), [](Person p) { return p.gender == Gender::FEMALE; });
        int result = ComputeMedianAge(persons.begin(), it);
        std::cout << "Median age for females = " << result << std::endl;
    }
    { //males
        auto it = std::partition(persons.begin(), persons.end(), [](Person p) { return p.gender == Gender::MALE; });
        int result = ComputeMedianAge(persons.begin(), it);
        std::cout << "Median age for males = " << result << std::endl;
    }
    { //employed females
        auto it = std::partition(persons.begin(), persons.end(), 
            [](Person p) { return ( p.gender == Gender::FEMALE && p.is_employed ); });
        int result = ComputeMedianAge(persons.begin(), it);
        std::cout << "Median age for employed females = " << result << std::endl;
    }
    { //unemployed females
        auto it = std::partition(persons.begin(), persons.end(),
            [](Person p) { return (p.gender == Gender::FEMALE && !p.is_employed); });
        int result = ComputeMedianAge(persons.begin(), it);
        std::cout << "Median age for unemployed females = " << result << std::endl;
    }
    { //employed males
        auto it = std::partition(persons.begin(), persons.end(), 
            [](Person p) { return (p.gender == Gender::MALE && p.is_employed); });
        int result = ComputeMedianAge(persons.begin(), it);
        std::cout << "Median age for employed males = " << result << std::endl;
    }
    { //employed males
        auto it = std::partition(persons.begin(), persons.end(),
            [](Person p) { return (p.gender == Gender::MALE && !p.is_employed); });
        int result = ComputeMedianAge(persons.begin(), it);
        std::cout << "Median age for unemployed males = " << result << std::endl;
    }
}

int main() {
    vector<Person> persons = {
        {31, Gender::MALE, false},
        {40, Gender::FEMALE, true},
        {24, Gender::MALE, true},
        {20, Gender::FEMALE, true},
        {80, Gender::FEMALE, false},
        {78, Gender::MALE, false},
        {10, Gender::FEMALE, false},
        {55, Gender::MALE, true},
    };
    PrintStats(persons);
    return 0;
}
