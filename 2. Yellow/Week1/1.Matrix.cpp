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
class Matrix
{
public:
	Matrix() : rowsNum(0), columnNum(0) {}
	Matrix( int a, int b ) {
		if ( a < 0 || b < 0 ) {
			throw out_of_range("");
		}
		Reset(a, b);
	}
	void Reset(int a, int b) {
		if ( a < 0 || b < 0 ) {
			throw out_of_range("");
		}
		rowsNum = a;
		columnNum = b;
		vector<int> tmp(b, 0);
		values.resize(a, tmp);
		for( int i = 0; i < a; ++i ) {
			for( int j = 0; j < b; ++j ) {
				values[i][j] = 0;
			}
		}
	}

	int At(int a, int b) const {
		if ( a < 0 || b < 0 || a >= rowsNum || b >= columnNum ) {
			throw out_of_range("");
		} else {
			//cout << "const method\n";
			return values[a][b];
		}
	}
	int& At(int a, int b) {
		if ( a < 0 || b < 0 || a >= rowsNum || b >= columnNum ) {
			throw out_of_range("");
		} else {
			return values[a][b];
		}
	}

	int GetNumRows() const {
		return rowsNum;
	}
	int GetNumColumns() const{
		return columnNum;
	}

private:
	int rowsNum;
	int columnNum;
	vector<vector<int>> values;
};

istream& operator>>( istream& is, Matrix& m ) {
	int rows = 0, cols = 0;
	is >> rows >> cols;
	m.Reset(rows, cols);

	for( int i = 0; i < rows; ++i ) {
		for( int j = 0; j < cols; ++j ) {
			int tmp = 0;
			is >> tmp;
			m.At(i, j) = tmp;
		}
	}
	return is;
}
ostream& operator<<(ostream& os, const Matrix& m) {
	os << m.GetNumRows() << " " << m.GetNumColumns() << endl;
	for( int i = 0; i < m.GetNumRows(); ++i ) {
		for( int j = 0; j < m.GetNumColumns(); ++j ) {
			os << m.At(i, j);
			if ( j != m.GetNumColumns()-1 ) {
				os << " ";
			}
		}
		os << endl;
	}
	return os;
}

bool operator==(const Matrix& m1, const Matrix& m2) {
	if ( m1.GetNumRows() != m2.GetNumRows() ||
			m1.GetNumColumns() != m2.GetNumColumns() ) {
		return false;
	}
	for( int i = 0; i < m1.GetNumRows(); ++i ) {
		for( int j = 0; j < m1.GetNumColumns(); ++j ) {
			if( m1.At(i, j) != m2.At(i, j) ) {
				return false;
			}
		}
	}
	return true;
}

Matrix operator+(const Matrix& m1, const Matrix& m2) {
	if ( m1.GetNumRows() != m2.GetNumRows() ||
			m1.GetNumColumns() != m2.GetNumColumns() ) {
		throw invalid_argument("");
	}
	Matrix res(m1.GetNumRows(), m1.GetNumColumns());
	for( int i = 0; i < m1.GetNumRows(); ++i ) {
		for( int j = 0; j < m1.GetNumColumns(); ++j ) {
			res.At(i, j) = m1.At(i, j) + m2.At(i, j);
		}
	}
	return res;
}
/*
int main() {
	try
	{
		  Matrix one, two, three;

		  cin >> one;

		  //one.Reset(3, 3);
		  cout << one;
	}
	catch( exception& ex ) {
		cout << "exception caught:\n";
		cout << ex.what();
	}

  return 0;
}*/
