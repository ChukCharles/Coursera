#include "date.h"
#include <vector>

Date ParseDate(istream& date) {
  //istream date_stream = date;
  bool ok = true;

  int year;
  ok = ok && (date >> year);
  ok = ok && (date.peek() == '-');
  date.ignore(1);

  int month;
  ok = ok && (date >> month);
  ok = ok && (date.peek() == '-');
  date.ignore(1);

  int day;
  ok = ok && (date >> day);
  //ok = ok && date.eof();

  if (!ok) {
    throw logic_error("Wrong date format");
  }
  return Date(year, month, day);
}

ostream& operator<<(ostream& stream, const Date& date) {
	stream << setw(4) << setfill('0') << date.GetYear() <<
		"-" << setw(2) << setfill('0') << date.GetMonth() <<
		"-" << setw(2) << setfill('0') << date.GetDay();
	return stream;
}

bool operator==( const Date& lhs, const Date& rhs ) {
	return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} ==
			vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}
bool operator!=( const Date& lhs, const Date& rhs ) {
	return !( lhs == rhs );
}
bool operator>( const Date& lhs, const Date& rhs ) {
	return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} >
			vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}
bool operator>=( const Date& lhs, const Date& rhs ) {
	return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} >=
			vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}
bool operator<( const Date& lhs, const Date& rhs ) {
	return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <
			vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}
bool operator<=( const Date& lhs, const Date& rhs ) {
	return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <=
			vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}
