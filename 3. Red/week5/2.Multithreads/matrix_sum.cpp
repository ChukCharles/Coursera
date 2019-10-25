#include "test_runner.h"
#include <vector>
#include <future>
#include <numeric>

using namespace std;

int64_t CalculateMatrixSum(const vector<vector<int>> &matrix) {
    int threads_num = 4;
    int64_t result = 0;
    vector<future<int64_t>> futures;
    int step_size = max( int(matrix.size()/threads_num+1), 1 );

    for( int i = 0; i < threads_num; ++i )
    {
        int fr = step_size*i;
        int lr = step_size*(i+1);
        lr = ( lr > matrix.size() ) ? matrix.size() : lr;
        if( fr == lr ) {
            break;
        }

        futures.push_back(async([&matrix, fr, lr] {
            int64_t res = 0;
            for( int j = fr; j < lr; ++j ) {
                res += accumulate( matrix[j].begin(), matrix[j].end(), 0 );
            }
            return res;
        }));
    }
    for( auto& f : futures ) {
        result += f.get();
        cout << result << " " << endl;
    }
    return result;    
}

void TestCalculateMatrixSum() {
    const vector<vector<int>> matrix = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };
    ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestCalculateMatrixSum);
}
