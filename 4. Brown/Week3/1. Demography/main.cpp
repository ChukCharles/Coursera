#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <map>
#include <fstream>

using namespace std;



struct Person {
    string name;
    int age, income;
    char sex;
};

class Database {
public:
    Database(vector<Person> p) : data(p) {
        for (Person& p : data) {
            ages.push_back(p.age);
            incomes.push_back(p.income);
            names[p.sex][p.name]++;
        }
        sort(ages.begin(), ages.end());
        sort(incomes.begin(), incomes.end());
        for (auto gender_names : names) {
            string result;
            int max = 0;
            for (pair<string, int> np : gender_names.second) {
                if (np.second > max) {
                    result = np.first;
                    max = np.second;
                }
                else if (np.second == max) {
                    result = (result < np.first) ? result : np.first;
                }
            }
            pop_names[gender_names.first] = result;
        }
    }
    int CountAdults(int adult_age) const;
    int CountWealth(size_t welth_count) const;
    string MostPopularNameForGender(char g) const;
private:
    vector<Person> data;
    vector<int> ages;
    vector<int> incomes;
    map<char, map<string, int>> names;
    map<char, string> pop_names;
};

int Database::CountAdults(int adult_age) const {
    int result;
    auto it = lower_bound(ages.begin(), ages.end(), adult_age);
    result = std::distance(it, end(ages));
    return result;
}
int Database::CountWealth(size_t welth_count) const {
    vector<int>::const_iterator first = prev(incomes.end(), min(welth_count, incomes.size()));
    return accumulate(first, incomes.end(), 0);
}

string Database::MostPopularNameForGender(char g) const
{
    if (pop_names.count(g) == 0) return "";
    return pop_names.at(g);
}

vector<Person> ReadPeople(istream& input) {
    int count;
    input >> count;

    vector<Person> result(count);
    for (Person& p : result) {
        char gender;
        input >> p.name >> p.age >> p.income >> gender;
        if (gender == 'M' || gender == 'W') {
            p.sex = gender;
        }
        else {
            p.sex = 'Y';
        }
    }

    return result;
}

int main() {
    //ifstream input("demography.txt");
    vector<Person> p = ReadPeople(cin);
    const Database db(p);



    for (string command; cin >> command; ) {
        if (command == "AGE") {
            int adult_age;
            cin >> adult_age;

            cout << "There are " << db.CountAdults(adult_age)
                << " adult people for maturity age " << adult_age << '\n';
        }
        else if (command == "WEALTHY") {
            int count;
            cin >> count;

            
            cout << "Top-" << count << " people have total income " << db.CountWealth(count) << '\n';
        }
        else if (command == "POPULAR_NAME") {
            char gender;
            cin >> gender;

            string name = db.MostPopularNameForGender(gender);
            if (name == "") {
                cout << "No people of gender " << gender << '\n';
            }
            else {
                
                cout << "Most popular name among people of gender " << gender << " is "
                    << name << '\n';
            }
        }
    }
}
