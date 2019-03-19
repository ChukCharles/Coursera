#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>

using namespace std;

// Реализуйте здесь
// * класс Matrix
// * оператор ввода для класса Matrix из потока istream
// * оператор вывода класса Matrix в поток ostream
// * оператор проверки на равенство двух объектов класса Matrix
// * оператор сложения двух объектов класса Matrix
class Matrix {
public:
	Matrix() : rowsNum(0), columnNum(0) {}
	Matrix( int a, int b ) rowsNum(a), columnNum(b) {
	if ( a < 0 || b < 0 ) {
		throw out_of_range("");
	}
	void Reset(int a, int b) {
		rowsNum = a;
		columnNum = b;
	}

	int At(int a, int b) const {
		if ( a < 0 || b < 0 || a > rowsNum || b > columnNum ) {
			throw out_of_range("");
		} else {
			return values[a][b];
		}
	}
	int& At(int a, int b) {
		if ( a < 0 || b < 0 || a > rowsNum || b > columnNum ) {
			throw out_of_range("");
		} else {
			return &values[a][b];
		}
	}

	int GetNumRows() const {
		return rowsNum;
	}
	int GetNumColumns() const {
		return columnNum;
	}

	//
private:
	int rowsNum;
	int columnNum;
	vector<vector<int>> values;
};

int main() {
  Matrix one(3,5);
  Matrix two(-1,3);

  //cin >> one >> two;
  //cout << one + two << endl;
  return 0;
}
