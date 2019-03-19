#include <vector>
#include <set>
#include <iostream>
#include <algorithm>

using namespace std;

template <typename T>
vector<T> FindGreaterElements(const set<T>& elements, const T& border) {
	vector<T> res(0);
	auto brd = find_if( elements.begin(), elements.end(), [&border]( const T& el ) { return el > border; } );
	if ( brd == elements.end() ) return res;

	for( ; brd != elements.end(); ++brd ) {
		res.push_back(*brd);
	}
	return res;
}

int main() {
  for (int x : FindGreaterElements(set<int>{1, 5, 7, 8}, 6)) {
    cout << x << " ";
  }
  cout << endl;

  string to_find = "Python";
  cout << FindGreaterElements(set<string>{"Ruby", "C", "C++"}, to_find).size() << endl;
  return 0;
}

