#include <cstdlib>
#include <vector>
#include <utility>
#include "test_runner.h"

using namespace std;

template <typename T>
class Table {
    public:
        Table();
        Table(size_t r, size_t c) : _rows(r), _columns(c) {
            vector<T> tmp(c);
            for (int i = 0; i < r; i++) {
                _data.push_back(tmp);
            }
        }
        
        void printTable() const {
            for (const auto& row : _data) {
                for (const auto& item : row) {
                    std::cout << item << " ";
                }
                std::cout << endl;
            }
        }
        
        void Resize(size_t r, size_t c) {
            _data.resize(r);
            for(auto& row : _data) {
                row.resize(c);
            }
            _rows = r;
            _columns = c;
        }
        
        vector<T>& operator[](size_t index) {
            return _data[index];
        }
        const vector<T> operator[](size_t index) const {
            return _data[index];
        }
        
        pair<size_t, size_t> Size() const {
            return make_pair(_rows, _columns);
        }
        
        size_t rows() const { return _rows; }
        size_t columns() const { return _columns; }
    private:
        vector<vector<T>> _data;
        size_t _rows;
        size_t _columns;
};


int main() {
    Table<int> test(3, 5);
    test.printTable();

    //
    //test.printTable();

    cout << "rows: " << test.rows() << endl
            << "columns: " << test.columns() << endl;
    
    for(int i = 0; i < test.rows(); i++) {
        for (int j = 0; j < test.columns(); j++) {
            test[i][j] = 10*i + j;
        }
    }
    
    test.Resize(2, 3);
    pair<size_t, size_t> sz = test.Size();
    cout << "rows: " << sz.first << endl
        << "columns: " << sz.second << endl;
    
    test.printTable();
    

    /*vector<vector<int>> matrix;
    for (int i = 0; i < 3; i++) {
        vector<int> tmp;
        for (int j = 0; j < 10; j++) {
            tmp.push_back(i*10 + j);
        }
        matrix.push_back(tmp);
    }*/
    
    //cout << matrix[2][3] << endl;
    
    
    /*for ( auto& x : matrix ) {
        for ( auto& y : x ) {
            cout << y << ", " ;
        }
        cout << endl;
    }*/

    return 0;
}
