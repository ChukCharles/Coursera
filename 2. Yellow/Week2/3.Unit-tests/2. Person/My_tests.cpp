#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stdexcept>

using namespace std;

template <class T>
ostream& operator<<(ostream& os, const vector<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class T>
ostream& operator<<(ostream& os, const set<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class K, class V>
ostream& operator<<(ostream& os, const map<K, V>& m) {
  os << "{";
  bool first = true;
  for (const auto& kv : m) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << kv.first << ": " << kv.second;
  }
  return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
  if (t != u) {
    ostringstream os;
    os << "Assertion failed: " << t << " != " << u;
    if (!hint.empty()) {
      os << " hint: " << hint;
    }
    throw runtime_error(os.str());
  }
}

void Assert(bool b, const string& hint) {
  AssertEqual(b, true, hint);
}

class TestRunner {
public:
  template <class TestFunc>
  void RunTest(TestFunc func, const string& test_name) {
    try {
      func();
      cerr << test_name << " OK" << endl;
    } catch (exception& e) {
      ++fail_count;
      cerr << test_name << " fail: " << e.what() << endl;
    } catch (...) {
      ++fail_count;
      cerr << "Unknown exception caught" << endl;
    }
  }

  ~TestRunner() {
    if (fail_count > 0) {
      cerr << fail_count << " unit tests failed. Terminate" << endl;
      exit(1);
    }
  }

private:
  int fail_count = 0;
};
/*
// если имя неизвестно, возвращает пустую строку
string FindNameByYear(const map<int, string>& names, int year) {
  string name;  // изначально имя неизвестно
  
  // перебираем всю историю по возрастанию ключа словаря, то есть в хронологическом порядке
  for (const auto& item : names) {
    // если очередной год не больше данного, обновляем имя
    if (item.first <= year) {
      name = item.second;
    } else {
      // иначе пора остановиться, так как эта запись и все последующие относятся к будущему
      break;
    }
  }
  
  return name;
}

class Person {
public:
  void ChangeFirstName(int year, const string& first_name) {
    first_names[year] = first_name;
  }
  void ChangeLastName(int year, const string& last_name) {
    last_names[year] = last_name;
  }
  string GetFullName(int year) {
    // получаем имя и фамилию по состоянию на год year
    const string first_name = FindNameByYear(first_names, year);
    const string last_name = FindNameByYear(last_names, year);
    
    // если и имя, и фамилия неизвестны
    if (first_name.empty() && last_name.empty()) {
      return "Incognito";
    
    // если неизвестно только имя
    } else if (first_name.empty()) {
      return last_name + " with unknown first name";
      
    // если неизвестна только фамилия
    } else if (last_name.empty()) {
      return first_name + " with unknown last name";
      
    // если известны и имя, и фамилия
    } else {
      return first_name + " " + last_name;
    }
  }

private:
  map<int, string> first_names;
  map<int, string> last_names;
};
*/
void TestEmpty() {
    Person p;
    AssertEqual( p.GetFullName(1999), "Incognito", "Test empty person" );
    p.ChangeLastName( 1999, "Volkov" );
    AssertEqual( p.GetFullName(1999), "Volkov with unknown first name", "Test person without first_name" );
    p.ChangeFirstName( 1990, "Ilya" );
    AssertEqual( p.GetFullName(1990), "Ilya with unknown last name", "Test person without last_name" );
}

void TestFilled() {
    Person p;
    p.ChangeFirstName( 1999, "Vasya" );
    p.ChangeFirstName( 1990, "Ilya" );
    p.ChangeLastName( 1993, "Volkov" );
    AssertEqual( p.GetFullName(1992), "Ilya with unknown last name", "Test filled person 1_1");
    AssertEqual( p.GetFullName(1993), "Ilya Volkov", "Test filled person 1_2");
    AssertEqual( p.GetFullName(1994), "Ilya Volkov", "Test filled person 1_3");
    p.ChangeLastName( 1995, "Shishkov" );
    AssertEqual( p.GetFullName(1994), "Ilya Volkov", "Test filled person 2_1");
    AssertEqual( p.GetFullName(1995), "Ilya Shishkov", "Test filled person 2_2");
    AssertEqual( p.GetFullName(1996), "Ilya Shishkov", "Test filled person 2_3");
    AssertEqual( p.GetFullName(1998), "Ilya Shishkov", "Test filled person 3_1");
    AssertEqual( p.GetFullName(1999), "Vasya Shishkov", "Test filled person 3_2");
    AssertEqual( p.GetFullName(2000), "Vasya Shishkov", "Test filled person 3_3");
}

int main() {
  TestRunner runner;
  runner.RunTest(TestEmpty, "TestEmpty");
  runner.RunTest(TestFilled, "TestFilled");

  return 0;
}