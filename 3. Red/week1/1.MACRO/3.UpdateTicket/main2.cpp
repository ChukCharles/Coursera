#include "airline_ticket.h"
#include "test_runner.h"
#include <sstream>
#include <string>


bool operator<(const Date& lhs, const Date& rhs) {
    if (lhs.year < rhs.year) return true;
    else if (lhs.year > rhs.year) return false;

    if (lhs.month < rhs.month) return true;
    else if (lhs.month > rhs.month) return false;

    if (lhs.day < rhs.day) return true;
    else return false;
}
bool operator==(const Date& lhs, const Date& rhs) {
    if (lhs.year == rhs.year &&
        lhs.month == rhs.month &&
        lhs.day == rhs.day) {
        return true;
    }
    else {
        return false;
    }
}
ostream& operator << (ostream& os, const Date& d) {
    os << d.year << "-" << d.month << "-" << d.day;
    return os;
}
istream& operator >> (istream& is, Date& d) {
    char c;
    is >> d.year >> c >> d.month >> c >> d.day;
    return is;
}

bool operator<(const Time& lhs, const Time& rhs) {
    if (lhs.hours < rhs.hours) return true;
    else if (lhs.hours > rhs.hours) return false;

    if (lhs.minutes < rhs.minutes) return true;
    else return false;
}
bool operator==(const Time& lhs, const Time& rhs) {
    if (lhs.hours == rhs.hours &&
        lhs.minutes == rhs.minutes) {
        return true;
    }
    else {
        return false;
    }
}
ostream& operator << (ostream& os, const Time& t) {
    os << t.hours << ":" << t.minutes;
    return os;
}
istream& operator >> (istream& is, Time& t) {
    char c;
    is >> t.hours >> c >> t.minutes;
    return is;
}

using namespace std;
// Реализуйте этот макрос, а также необходимые операторы для классов Date и Time
#define UPDATE_FIELD(ticket, field, values)  { \
    if (values.count(#field) > 0) { \
        istringstream is(values.at(#field));        \
        is >> ticket.field; \
    } \
}

void TestUpdate() {
  AirlineTicket t;
  t.price = 0;

  const map<string, string> updates1 = {
    {"departure_date", "2018-2-28"},
    {"departure_time", "17:40"},
  };
  UPDATE_FIELD(t, departure_date, updates1);
  UPDATE_FIELD(t, departure_time, updates1);
  UPDATE_FIELD(t, price, updates1);

  ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
  ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
  ASSERT_EQUAL(t.price, 0);

  const map<string, string> updates2 = {
    {"price", "12550"},
    {"arrival_time", "20:33"},
  };
  UPDATE_FIELD(t, departure_date, updates2);
  UPDATE_FIELD(t, departure_time, updates2);
  UPDATE_FIELD(t, arrival_time, updates2);
  UPDATE_FIELD(t, price, updates2);

  // updates2 не содержит ключей "departure_date" и "departure_time", поэтому
  // значения этих полей не должны измениться
  ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
  ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
  ASSERT_EQUAL(t.price, 12550);
  ASSERT_EQUAL(t.arrival_time, (Time{20, 33}));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestUpdate);
}
