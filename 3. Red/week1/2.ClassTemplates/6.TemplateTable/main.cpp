#include "test_runner.h"

#include <string>
#include <vector>
#include <iostream>
#include <utility>
using namespace std;

template<typename T>
class Table {
public:
    Table(size_t x, size_t y) : table_(x, vector<T>(y)) {}
    pair<size_t,size_t> Size() const { 
        return make_pair( table_.size(), table_[0].size() ); }
    vector<T>& operator[](size_t x) {
        return table_[x];
    }
    vector<T> operator[](size_t x) const {
        return table_.at(x);
    }
    void Resize(size_t x, size_t y) {
        table_.resize( x );
        for( auto& i : table_) {
            i.resize( y );
        }
    }
private:
    vector<vector<T>> table_;
};

template<typename T>
ostream& operator<<( ostream& os, const Table<T>& t) {
    
}
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
  RUN_TEST(tr, TestTable);
  return 0;
}