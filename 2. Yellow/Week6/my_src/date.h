#pragma once

#include <sstream>
#include <iomanip>

using namespace std;

class Date {
public:
  Date(int new_year, int new_month, int new_day) {
    year = new_year;
    if (new_month > 12 || new_month < 1) {
      throw logic_error("Month value is invalid: " + to_string(new_month));
    }
    month = new_month;
    if (new_day > 31 || new_day < 1) {
      throw logic_error("Day value is invalid: " + to_string(new_day));
    }
    day = new_day;
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

Date ParseDate(istream& date);

ostream& operator<<( ostream& os, const Date& d );

bool operator==( const Date& a, const Date& b );
bool operator!=( const Date& a, const Date& b );
bool operator>( const Date& a, const Date& b );
bool operator>=( const Date& a, const Date& b );
bool operator<( const Date& a, const Date& b );
bool operator<=( const Date& a, const Date& b );
