#include "sum_reverse_sort.h"
#include <algorithm>

int Sum(int x, int y) {
	return x + y;
}
string Reverse(string s) {
	if( s.length() < 2 ) return s;
	string result = "";
	for( unsigned i = 0; i < s.length(); ++i ) {
		result += s[s.length()-1-i];
	}
	return result;
}
void Sort(vector<int>& nums) {
	sort( begin(nums), end(nums) );
}
