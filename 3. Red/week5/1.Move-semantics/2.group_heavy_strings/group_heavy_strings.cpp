#include "test_runner.h"

#include <string>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <utility>
#include <algorithm>
using namespace std;

// Объявляем Group<String> для произвольного типа String
// синонимом vector<String>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор групп — vector<Group<String>>.
template <typename String>
using Group = vector<String>;

// Ещё один шаблонный синоним типа
// позволяет вместо громоздкого typename String::value_type
// использовать Char<String>
template <typename String>
using Char = typename String::value_type;

template <typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
    // Напишите реализацию функции,
    // использовав не более 1 копирования каждого символа
    vector<Group<String>> result;
    list<String> pool;
    for (auto it = strings.begin(); it != strings.end(); ++it) {
      pool.push_back(move(*it));
    }
    
    map<set<Char<String>>, Group<String>> g_map;
    while( !pool.empty() ) {
        auto first = pool.begin();
        set<Char<String>> key;
        for( Char<String>& c : *first ) {
            key.insert(c);
        }
        g_map[move(key)].push_back(move(*first));
        pool.erase(first);
    }
    for( auto it = g_map.begin(); it != g_map.end(); ++it ) {
        result.push_back(move(it->second));
    }
    return result;
}

void TestGroupingABC() {
    vector<string> strings = {"caab", "abc", "cccc", "bacc", "c"};
    auto groups = GroupHeavyStrings(strings);
    ASSERT_EQUAL(groups.size(), 2);
    sort(begin(groups), end(groups)); // Порядок групп не имеет значения
    ASSERT_EQUAL(groups[0], vector<string>({"caab", "abc", "bacc"}));
    ASSERT_EQUAL(groups[1], vector<string>({"cccc", "c"}));
}

void TestGroupingReal() {
    vector<string> strings = {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl"};
    auto groups = GroupHeavyStrings(strings);
    ASSERT_EQUAL(groups.size(), 4);
    sort(begin(groups), end(groups)); // Порядок групп не имеет значения
    ASSERT_EQUAL(groups[0], vector<string>({"laptop", "paloalto"}));
    ASSERT_EQUAL(groups[1], vector<string>({"law", "wall", "awl"}));
    ASSERT_EQUAL(groups[2], vector<string>({"port"}));
    ASSERT_EQUAL(groups[3], vector<string>({"top", "pot"}));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestGroupingABC);
    RUN_TEST(tr, TestGroupingReal);

    return 0;
}