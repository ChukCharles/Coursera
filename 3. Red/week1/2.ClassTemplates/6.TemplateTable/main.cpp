#include "test_runner.h"
#include <vector>
#include <string>

using namespace std;

// Реализуйте здесь шаблонный класс Table
template <typename T>
class Table {
public:
	Table(size_t x, size_t y) : data(x, vector<T>(y)) {}

	vector<T>& operator[](size_t x) { return data[x]; }
	const vector<T>& operator[](size_t x) const { return data.at(x); }

	void Resize(size_t x, size_t y) {
		if (x == 0 || y == 0) {
			data.clear();
			return;
		}
		data.resize(x, vector<T>(y));
		for (auto& line : data) { line.resize(y); }
		return;
	}

	pair<size_t, size_t> Size() const {
		//if (data.empty() || data[0].empty()) return { 0, 0 };
		if (data.size() == 0 || data[0].size() == 0) return { 0, 0 };
		else return { data.size(), data[0].size() };
	}

private:
	vector<vector<T>> data;
};

void TestTable() {
  Table<int> t(1, 1);
  ASSERT_EQUAL(t.Size().first, 1u);
  ASSERT_EQUAL(t.Size().second, 1u);
  t[0][0] = 42;
  ASSERT_EQUAL(t[0][0], 42);
  t.Resize(3, 4);
  ASSERT_EQUAL(t.Size().first, 3u);
  ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
  TestRunner tr;
  //RUN_TEST(tr, TestTable);

  Table<vector<string>> t(5, 7);
  t.Resize(5, 7);
  cout << t.Size().first << " " << t.Size().second << endl;

  return 0;
}
