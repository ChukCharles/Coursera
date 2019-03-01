#include <iostream>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include <string>
#include <sstream>
#include <map>
#include <set>
using namespace std;

class Date {
public:
    Date() : year(0), month(0), day(0) {}
    Date(int y, int m, int d) {
        if (m > 12 || m < 1 ) {
            string error = "Month value is invalid: " + to_string(m);
            throw runtime_error(error);
        }
        if ( d < 1 || d > 31 ) {
            string error = "Day value is invalid: " + to_string(d);
            throw runtime_error(error);
        }
        year = y;
        month = m;
        day = d;
    }
    int GetYear() const {
        return year;
    }
    int GetMonth() const {
        return month;
    }
    int GetDay() const {
        return day;
    }
private:
    int year;
    int month;
    int day;
};

bool operator<(const Date& lhs, const Date& rhs) {
    if ( lhs.GetYear() < rhs.GetYear() ) {
        return true;
    } else if ( lhs.GetYear() > rhs.GetYear() ) {
        return false;
    } else {
        if (lhs.GetMonth() < rhs.GetMonth()) {
            return true;
        } else if (lhs.GetMonth() > rhs.GetMonth()) {
            return false;
        } else {
            if (lhs.GetDay() < rhs.GetDay() ) {
                return true;
            } else {
                return false;
            }
        }
    }
}

ostream& operator<<( ostream& os, const Date& date ) {
    os << setfill('0') << setw(4) << date.GetYear() << "-"
            << setw(2) << date.GetMonth() << '-' << setw(2) << date.GetDay();
    return os;
}

istream& operator>>( istream& is, Date& date ) {
    string tmp;
    is >> tmp;
    stringstream ss;
    ss << tmp;
    int y, m, d;
    if ( ss >> y ) {
        ss.ignore(1);
        if ( ss >> m ) {
            ss.ignore(1);
            if ( ss >> d ) {
                if ( ss.eof() ) {
                    date = Date(y, m, d);
                    return is;
                }
            }
        }
    }
    throw runtime_error("Wrong date format: " + tmp);
}

class Database {
public:
    void AddEvent(const Date& date, const string& event) {
            events[date].insert(event);
    }
    bool DeleteEvent(const Date& date, const string& event) {
        if ( events.count(date) > 0 && events[date].count(event) ) {
            events[date].erase( event );
            return true;
        }
        return false;
    }
    int DeleteDate(const Date& date) {
        if ( events.count(date) > 0 ) {
            int n = events[date].size();
            events.erase( date );
            return n;
        }
        return 0;
    }

    set<string> Find(const Date& date) const {
        set<string> res;
        if ( events.count(date) > 0 ) {
            res = events.at(date);
        }
        return res;        
    }
    
    void Print() const {
        for ( const auto& e : events ) {
            for ( const auto& item : e.second ) {
                cout << e.first << " " << item << endl;
            }
        }
    }
private:
    map<Date, set<string>> events;
};

int main() {
    Database db;
    string command;
    
    while (getline(cin, command)) {
        // Считайте команды с потока ввода и обработайте каждую
        if ( command == "" ) {
            continue;
        }
        stringstream ss;
        ss << command;
        string comm;
        ss >> comm;
        try {
            Date t_date;
            if ( comm == "Add" ) {
                ss >> t_date;
                string event;
                ss >> event;
                db.AddEvent( t_date, event );
            } else if ( comm == "Del" ) {
                ss >> t_date;
                string ev;
                if (ss >> ev) {
                    if ( db.DeleteEvent( t_date, ev )) {
                        cout << "Deleted successfully\n";
                    } else {
                        cout << "Event not found\n";
                    }
                } else {
                    int n = db.DeleteDate( t_date );
                    cout << "Deleted " << n << " events\n";
                }
            } else if ( comm == "Find" ) {
                ss >> t_date;
                set<string> result = db.Find( t_date );
                for ( const string& r : result ) {
                    cout << r << endl;
                }
            } else if ( comm == "Print" ) {
                db.Print();
            } else {
                cout << "Unknown command: " << comm << endl;
            }
        }
        catch (runtime_error& re) {
            cout << re.what() << "\n";
        }
    }

    return 0;
}