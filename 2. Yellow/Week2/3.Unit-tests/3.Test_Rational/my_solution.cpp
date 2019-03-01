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

int GreatestCommonDivisor(int a, int b) {
  if (b == 0) {
    return a;
  } else {
    return GreatestCommonDivisor(b, a % b);
  }
}

class Rational {
public:
  Rational() {  // дробь по умолчанию — 0/1
    numerator = 0;
    denominator = 1;
  }
  Rational(int new_numerator, int new_denominator) {
    const int gcd = GreatestCommonDivisor(new_numerator, new_denominator);
    // сократим дробь, разделив числитель и знаменатель на их НОД
    numerator = new_numerator / gcd;
    denominator = new_denominator / gcd;
    // знаменатель должен быть положительным
    if (denominator < 0) {
      denominator = -denominator;
      numerator = -numerator;
    }
  }

  int Numerator() const {
    return numerator;
  }

  int Denominator() const {
    return denominator;
  }

private:
  int numerator;
  int denominator;
};

void TestEmptyConstructor() {
    Rational r;
    AssertEqual( r.Numerator(), 0, "Test default numerator" );
    AssertEqual( r.Denominator(), 1, "Test default denominator" );
}

void TestConstructorWithParam() {
    Rational r1(1,3);
    AssertEqual( r1.Numerator(), 1, "Test constructed numerator 1");
    AssertEqual( r1.Denominator(), 3, "Test constructed denominator 1");
    Rational r2(4,6);
    AssertEqual( r2.Numerator(), 2, "Test constructed numerator 2");
    AssertEqual( r2.Denominator(), 3, "Test constructed denominator 2");
}

void TestSign() {
    Rational r1(-1, 2);
    AssertEqual( r1.Numerator(), -1, "Test sign, numerator 1");
    AssertEqual( r1.Denominator(), 2, "Test sign, denominator 1");
    
    Rational r2(1, -2);
    AssertEqual( r2.Numerator(), -1, "Test sign, numerator 2");
    AssertEqual( r2.Denominator(), 2, "Test sign, denominator 2");

    Rational r3( 9, -21);
    AssertEqual( r3.Numerator(), -3, "Test sign, gcd, numerator 3");
    AssertEqual( r3.Denominator(), 7, "Test sign, gcd, denominator 3");
    
    Rational r4( -9, -21);
    AssertEqual( r4.Numerator(), 3, "Test sign, gcd, numerator 4");
    AssertEqual( r4.Denominator(), 7, "Test sign, gcd, denominator 4");
}

void TestZeroNum() {
    Rational r(0, 17);
    AssertEqual( r.Numerator(), 0, "Test zero num, numerator");
    AssertEqual( r.Denominator(), 1, "Test zero num, denominator");    
}

int main() {
  TestRunner runner;
  runner.RunTest(TestEmptyConstructor, "TestEmptyConstructor");
  runner.RunTest(TestConstructorWithParam, "TestConstructorWithParam");
  runner.RunTest(TestSign, "TestSign");
  runner.RunTest(TestZeroNum, "TestZeroNum");

  return 0;
}