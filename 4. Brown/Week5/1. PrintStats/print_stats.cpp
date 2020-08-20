#include<vector>
#include<algorithm>
#include<iostream>

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