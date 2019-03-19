#include <string>
#include <iostream>
#include <deque>
#include <algorithm>

using namespace std;

int main() {
	int initial_num = 0;
	cin >> initial_num;

	int op_num;
	cin >> op_num;

	deque<string> result;

	result.push_back( to_string( initial_num ));

	bool skip_parenthesis = true;

	for( int i = 0; i < op_num; i++ ) {
		string operation;
		int num = 0;
		cin >> operation >> num;

		if ( operation == "*" || operation == "/" ) {
			if ( !skip_parenthesis ) {
				result.push_front("(");
				result.push_back(")");
			}
			skip_parenthesis = true;
		} else {
			skip_parenthesis = false;
		}
		result.push_back(" " + operation + " "  + to_string(num));

	}

	for( auto it = result.begin(); it < result.end(); it++ ) {
		cout << *it;
	}

	return 0;
}

