#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "test_runner.h"
using namespace std;

enum class Gender {
    FEMALE,
    MALE
};

struct Person {
    int age;
    Gender gender;
    bool is_employed;
};

bool operator==(const Person& lhs, const Person& rhs) {
    return lhs.age == rhs.age
        && lhs.gender == rhs.gender
        && lhs.is_employed == rhs.is_employed;
}

ostream& operator<<(ostream& stream, const Person& person) {
    return stream << "Person(age=" << person.age
        << ", gender=" << static_cast<int>(person.gender)
        << ", is_employed=" << boolalpha << person.is_employed << ")";
}

struct AgeStats {
    int total;
    int females;
    int males;
    int employed_females;
    int unemployed_females;
    int employed_males;
    int unemployed_males;
};

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
    if (range_begin == range_end) {
        return 0;
    }
    vector<typename iterator_traits<InputIt>::value_type> range_copy(
        range_begin,
        range_end
    );
    auto middle = begin(range_copy) + range_copy.size() / 2;
    //auto middle = begin(range_copy) + (range_copy.size()+1) / 2;

    nth_element(
        begin(range_copy), middle, end(range_copy),
        [](const Person& lhs, const Person& rhs) {
            return lhs.age < rhs.age;
        }
    );
    return middle->age;
}

vector<Person> ReadPersons(istream& in_stream = cin) {
    int person_count;
    in_stream >> person_count;
    vector<Person> persons;
    persons.reserve(person_count);
    for (int i = 0; i < person_count; ++i) {
        int age, gender, is_employed;
        in_stream >> age >> gender >> is_employed;
        //in_stream >> age >> is_employed >> gender;
        Person person{
            age,
            static_cast<Gender>(gender),
            is_employed == 1
            //is_employed == 0
        };
        persons.push_back(person);
    }
    return persons;
}

AgeStats ComputeStats(vector<Person> persons) {
    //                 persons
    //                |       |
    //          females        males
    //         |       |      |     |
    //      empl.  unempl. empl.   unempl.

    auto females_end = partition(
        begin(persons), end(persons),
        [](const Person& p) {
            return p.gender == Gender::FEMALE;
        }
    );
    auto employed_females_end = partition(
        begin(persons), females_end,
        [](const Person& p) {
            return p.is_employed;
        }
    );
    auto employed_males_end = partition(
        females_end, end(persons),
        [](const Person& p) {
            return p.is_employed;
        }
    );

    return {
         ComputeMedianAge(begin(persons), end(persons)),
         ComputeMedianAge(begin(persons), females_end),
         ComputeMedianAge(females_end, end(persons)),
         ComputeMedianAge(begin(persons),  employed_females_end),
         //ComputeMedianAge(females_end, employed_females_end),
         ComputeMedianAge(employed_females_end, females_end),
         ComputeMedianAge(females_end, employed_males_end),
         ComputeMedianAge(employed_males_end, end(persons))
    };
}

void PrintStats(const AgeStats& stats,
    ostream& out_stream = cout) {
    out_stream << "Median age = "
        << stats.total << endl
        << "Median age for males = "
        << stats.males << endl
        << "Median age for females = "
        << stats.females << endl
        << "Median age for employed females = "
        << stats.employed_females << endl
        << "Median age for unemployed females = "
        << stats.unemployed_females << endl
        << "Median age for employed males = "
        << stats.employed_males << endl
        << "Median age for unemployed males = "
        << stats.unemployed_males << endl;
}

void TestEmptyVector() {
    vector<Person> empty;
    ASSERT_EQUAL(ComputeMedianAge(empty.begin(), empty.end()), 0);
}
void TestMedianComputing() {
    vector<Person> odd_size = {
        { 10, Gender::FEMALE, true },
        { 20, Gender::FEMALE, true },
        { 30, Gender::FEMALE, true }
    };
    ASSERT_EQUAL(ComputeMedianAge(odd_size.begin(), odd_size.end()), 20);
}
void TestReadingCorrectness() {
    std::istringstream iss("1\n30 0 1\n");
    vector<Person> expected = {
        { 30, Gender::FEMALE, true }
    };
    vector<Person> result = ReadPersons(iss);
    ASSERT_EQUAL(result, expected);
}

void TestIteratorCorrectness() {
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
    AgeStats expected = { 40, 40, 55, 40, 80, 55, 78 };
    AgeStats computed = ComputeStats(persons);

    //cout << computed.total
    //    << ", " << computed.females
    //    << ", " << computed.males
    //    << ", " << computed.employed_females
    //    << ", " << computed.unemployed_females
    //    << ", " << computed.employed_males
    //    << ", " << computed.unemployed_males << endl;
    ASSERT_EQUAL(computed.total, expected.total);
    ASSERT_EQUAL(computed.females, expected.females);
    ASSERT_EQUAL(computed.males, expected.males);
    ASSERT_EQUAL(computed.employed_females, expected.employed_females);
    ASSERT_EQUAL(computed.unemployed_females, expected.unemployed_females);
    ASSERT_EQUAL(computed.employed_males, expected.employed_males);
    ASSERT_EQUAL(computed.unemployed_males, expected.unemployed_males);
}
void TestPrinting() {
    AgeStats input = { 40, 40, 55, 40, 80, 55, 78 };
    std::string expected = 
        "Median age = 40\n"
        "Median age for females = 40\n"
        "Median age for males = 55\n"
        "Median age for employed females = 40\n"
        "Median age for unemployed females = 80\n"
        "Median age for employed males = 55\n"
        "Median age for unemployed males = 78\n";
    
    std::ostringstream oss;
    PrintStats(input, oss);

    ASSERT_EQUAL(oss.str(), expected);
}
int main() {
    //PrintStats(ComputeStats(ReadPersons()));
    TestRunner tr;
    RUN_TEST(tr, TestEmptyVector);
    RUN_TEST(tr, TestMedianComputing);
    RUN_TEST(tr, TestReadingCorrectness);
    RUN_TEST(tr, TestIteratorCorrectness);
    RUN_TEST(tr, TestPrinting);

    return 0;
}