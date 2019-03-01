#include <iostream>
#include <cstdint>
#include <vector>
#include <limits>
using namespace std;

int main() {
    int N=0, R=0;
    uint64_t sum=0;
    //cout << numeric_limits<int>::max() << endl;
    //cout << numeric_limits<double>::max() << endl;
    
    cin >> N >> R;
    for( int i=0; i<N; ++i ) {
        uint64_t w=0, h=0, l=0;
        cin >> w >> h >> l;
        sum += w*h*l*R;
    }
    cout << sum << endl;
    return 0;
}