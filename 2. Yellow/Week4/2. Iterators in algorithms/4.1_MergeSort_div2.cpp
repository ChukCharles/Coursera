#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <numeric>

using namespace std;
/*
void PrintVector( vector<int>& v ) {
	for ( int& x : v ) {
		cout << x << " ";
	}
	cout << endl;
}
using RandomIt = vector<int>::iterator;

void PrintRange( RandomIt b, RandomIt e ) {
	while( b != e ) {
		cout << *b << " ";
		b++;
	}
	cout << endl;
}
*/
template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
	vector<typename RandomIt::value_type> elements(range_begin, range_end);

	if( elements.size() > 2 ) {
		MergeSort( elements.begin(), elements.begin() + elements.size()/2 );
		MergeSort( elements.begin() + elements.size()/2, elements.end() );

	}

	merge( elements.begin(), elements.begin() + elements.size()/2,
			elements.begin() + elements.size()/2, elements.end(),
			range_begin);

}
/*
int main() {
	vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1};
	MergeSort(begin(v), end(v));
	for (int x : v) {
	cout << x << " ";
	}
	cout << endl;
	return 0;

	return 0;
}
*/