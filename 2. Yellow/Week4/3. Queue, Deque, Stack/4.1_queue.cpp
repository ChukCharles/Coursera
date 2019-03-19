#include <string>
#include <iostream>
#include <queue>

using namespace std;

int main() {
	int initial_num = 0;
	cin >> initial_num;

	int op_num;
	cin >> op_num;

	queue<string> result;
	for( int i =0; i < op_num; i++ ) {
		result.push("(");
	}
	result.push(to_string(initial_num));

	for( int i = 0; i < op_num; i++ ) {
		string operation;
		int num = 0;
		cin >> operation >> num;
		result.push( ") " + operation + " " + to_string(num) );
	}

	while( !result.empty() ) {
		cout << result.front();
		result.pop();
	}

	return 0;
}