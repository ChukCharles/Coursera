#include "test_runner.h"
#include "profile.h"

#include <map>
#include <list>
#include <algorithm>
using namespace std;

int main() {
    int N;
    cin >> N;
    list<int> pl_list;
    map<int, list<int>::iterator> pl_map;
    for( int i = 0; i < N; ++i ) {
        int curr, next;
        cin >> curr >> next;
        if( pl_map.count(next)>0 ) {
            auto pos = pl_map[next];
            pl_map[curr] = pl_list.insert(pos, curr);
        } else {
            pl_list.push_back(curr);
            pl_map[curr] = prev(pl_list.end());
            
        }
    }
    
    for( int x : pl_list ) {
        cout << x << " ";
    }
    cout << endl;

    return 0;
}
