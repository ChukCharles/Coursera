#include "airline_ticket.h"
#include "test_runner.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <tuple>
using namespace std;

bool operator==(const Date &l, const Date &r) {
    return tie(l.year, l.month, l.day) == tie(r.year, r.month, r.day);
}

bool operator<(const Date &l, const Date &r) {
    return tie(l.year, l.month, l.day) < tie(r.year, r.month, r.day);
}

ostream &operator<<(ostream &stream, const Date &date) {
    stream << setw(4) << setfill('0') << date.year << "-"
           << setw(2) << setfill('0') << date.month << "-"
           << setw(2) << setfill('0') << date.day;
    return stream;
}
istream &operator>>(istream &stream, Date &date) {
	char c;
	stream >> date.year >> c >> date.month >> c >> date.day;
    return stream;
}

bool operator==(const Time &l, const Time &r) {
    return tie(l.hours, l.minutes) == tie(r.hours, r.minutes);
}

bool operator<(const Time &l, const Time &r) {
    return tie(l.hours, l.minutes) < tie(r.hours, r.minutes);
}

ostream &operator<<(ostream &stream, const Time &time) {
    stream << time.hours << "-" << time.minutes;
    return stream;
}
istream &operator>>(istream &stream, Time &time) {
	char c;
	stream >> time.hours >> c >> time.minutes;
    return stream;
}

#define UPDATE_FIELD(ticket, field, values)	{	\
	map<string, string>::const_iterator it;		\
	it = values.find(#field);					\
	if (it != values.end()) {					\
	    istringstream is(it->second);			\
	    is >> ticket.field;						\
	}											\
}												\

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
