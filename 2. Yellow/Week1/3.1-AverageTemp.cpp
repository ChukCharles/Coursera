#include <iostream>
#include <cstdint>
#include <vector>
#include <limits>
using namespace std;

int main() {
    int N=0;
    int64_t sum=0;
    cin >> N;
    vector<int> temratures;
    for ( int i = 0; i < N; ++i ) {
        int tmp = 0;
        cin >> tmp;
        temratures.push_back(tmp);
        sum += tmp;
    }
    int avg = sum / N;
    //cout << avg;
    vector<int> result;
    for( int i = 0; i < N; ++i ) {
        if ( temratures[i] > avg ) {
            result.push_back( i );
        }
    }
    int rSize = static_cast<int>( result.size());
    cout << rSize << endl;
    for (int i=0; i < rSize; ++i) {
        cout << result[i];
        if( i != rSize-1 ) {
            cout << " ";
        }
    }
    return 0;
}