#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
using namespace std;

class Person {
public:
    Person(string fn, string ln, int y) : birthYear(y) {
        f_name[y] = fn;
        l_name[y] = ln;
    }

    void ChangeFirstName(int year, const string& first_name) {
        if ( year < birthYear ) return;
        f_name[year] = first_name;
    }

    void ChangeLastName(int year, const string& last_name) {
        if ( year < birthYear ) return;
        l_name[year] = last_name;
    }

    string GetFullName(int year) const {
        if ( year < birthYear ) {
            return "No person";
        }
        string first = getName(f_name, year);
        string last = getName(l_name, year);
        string result = "";
        if (first == "" && last == "") {
            result = "Incognito";
        } else if (first == "") {
            result = last + " with unknown first name";
        } else if (last == "") {
            result = first + " with unknown last name";
        } else {
            result = first + " " + last;
        }
        return result;
    }

    string GetFullNameWithHistory(int year) const {
        if ( year < birthYear ) {
            return "No person";
        }
        string first = getName(f_name, year);
        string last = getName(l_name, year);
        string result = "";
        if (first == "" && last == "") {
            result = "Incognito";
        }
        else if (first == "")
        {
            //result = last + " with unknown first name";
            string hist = getHistName(l_name, year);
            if (hist != "") {
                hist = " (" + hist + ')';
            }
            result = last + hist + " with unknown first name";
        }
        else if (last == "")
        {
            string hist = getHistName(f_name, year);
            if (hist != "") {
                hist = " (" + hist + ")";
            }
            result = first + hist + " with unknown last name";
        }
        else
        {
            string f_hist = getHistName(f_name, year);
            string l_hist = getHistName(l_name, year);
            if (f_hist != "") {
                f_hist = " (" + f_hist + ")";
            }
            if (l_hist != "") {
                l_hist = " (" + l_hist + ")";
            }
            result = first + f_hist + " " + last + l_hist;
        }
        return result;
    }
private:
    map<int, string> f_name;
    map<int, string> l_name;
    const int birthYear;

    string getName(const map<int, string>& m, int year) const {
        string res = "";
        for (const auto& i : m) {
            if (i.first <= year) {
                res = i.second;
            } else {
                break;
            }
        }

        return res;
    }

    string getHistName(const map<int, string>& m, int year) const {
        string res = "";
        vector<string> r;
        for (const auto& i : m) {
            if (i.first <= year) {
                r.push_back(i.second);
            }
        }
        if ( r.size() > 0 )
        {
            for (int i = r.size() - 2; i >= 0; --i) {
                if ( r[i] != r[i+1] ) {
                    res += r[i];
                    if (i > 0 ) {
                        res += ", ";
                    }
                }

            }
        }
        return res;
    }

};

int main() {
  Person person("Polina", "Sergeeva", 1960);
  for (int year : {1959, 1960}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }
  
  person.ChangeFirstName(1965, "Appolinaria");
  person.ChangeLastName(1967, "Ivanova");
  for (int year : {1965, 1967}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  return 0;
}