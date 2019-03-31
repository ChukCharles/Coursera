#include "test_runner.h"

#include <ostream>
#include <iostream>
using namespace std;


//Original statement:
//#define PRINT_VALUES(out, x, y) out << (x) << endl; out << (y) << endl
//Do not work if we use if-else statement without {} (see example below)

#define PRINT_VALUES(out, x, y) out << (x) << endl << (y) << endl

int main() {
	TestRunner tr;
	tr.RunTest([] {
	ostringstream output;
	PRINT_VALUES(output, 5, "red belt");
	ASSERT_EQUAL(output.str(), "5\nred belt\n");
	}, "PRINT_VALUES usage example");

	int x = 5;
	if( x > 3 ) PRINT_VALUES(cout, x, "red belt");
	else {
	  cout << "wow\n";
	}

	/*with original statement we will get:
	 * if( x > 3 ) out << (x) << endl; out << (y) << endl;
	 * else {
	 *    cout << "wow\n";
	 * }
	 */

	return 0;
}

