#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <stdexcept>

using namespace std;

template <typename T>
T Sqr( T value ) {
    return value*value;
}

template <typename T>
vector<T> operator*( const vector<T>& v1, const vector<T>& v2 ) 
{
    if( v1.size() != v2.size() ) {
        throw runtime_error("");
    }
    vector<T> result(v1.size());
    for( int i = 0; i < v1.size(); ++i ) {
        result[i] = v1.at(i)*v2.at(i);
    }
    return result;
}

template <typename Key, typename Value>
map<Key, Value> operator*( const map<Key, Value>& m1, const map<Key, Value>& m2 ) {
    //int r_size = ( m1.size() != m2.size() ) ? m1.size() : throw runtime_error("");
    map<Key, Value> result;
    for( const auto& p : m1 ) {
        result[p.first] = m1.at(p.first)*m2.at(p.first);
    }
    return result;
}

template <typename Key, typename Value>
pair<Key, Value> operator*( const pair<Key, Value>& p1, const pair<Key, Value>& p2 ) {
    if( p1.first != p2.first ) throw runtime_error("");
    return make_pair(p1.first*p1.first, p1.second*p2.second);
}

int main() {
    cout << Sqr<double>(5.07);
    vector<int> v = {1, 2, 3};
    cout << "vector:";
    for (int x : Sqr(v)) {
      cout << ' ' << x;
    }
    cout << endl;

    map<int, pair<int, int>> map_of_pairs = {
      {4, {2, 2}},
      {7, {4, 3}}
    };
    cout << "map of pairs:" << endl;
    for (const auto& x : Sqr(map_of_pairs)) {
      cout << x.first << ' ' << x.second.first << ' ' << x.second.second << endl;
    }
    return 0;
}