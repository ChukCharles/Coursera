#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

void PrintVectorPart(const vector<int>& numbers) {
	vector<int>::const_iterator f_neg = find_if( numbers.begin(), numbers.end(),
										[]( const int& n) { return n < 0; });
	while( f_neg != numbers.begin() ) {
		 --f_neg;
		cout << *f_neg << " ";
	}

	/* alternative way to iterate:
	for (auto it = negative_it; it != numbers.begin(); ) {
    	cout << *(--it) << " ";
	}
	*/
	cout << endl;
}
/*
int main() {
	PrintVectorPart({6, 1, 8, -5, 4});
	PrintVectorPart({-6, 1, 8, -5, 4});  // ничего не выведется
	PrintVectorPart({6, 1, 8, 5, 4});
	return 0;
}
*/
