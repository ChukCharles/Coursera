#include <cstdlib>
#include <vector>
#include <utility>
#include  <algorithm>
#include <iterator>
#include "test_runner.h"

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator f, Iterator l) : first(f), last(l) {}
    
    Iterator begin() {
        return first;
    }
    Iterator end() {
        return last;
    }
    
private:
    Iterator first;
    Iterator last;
};

template<typename Container>
auto Head(Container& c, size_t top) {
    IteratorRange tmp = {
        c.begin(), next(c.begin(), std::min(top, c.size()))
    };
    cout << "Head: result container: ";
    for( auto x : tmp ) {
        cout << x << " ";
    }
    cout << endl;
    return tmp;
}

int main() {
    const vector<int> vi = {1,2,3,5,7};
    cout << "Initial vector: ";
    for( auto& x : vi ) {
        cout << x << " ";
    }
    cout << endl;
    IteratorRange<vector<int>::const_iterator> ir1(vi.begin(), vi.end());
//    IteratorRange ir(vi.begin(), vi.end());
    
    IteratorRange ir3 = Head(vi, 3);
    cout << "main: result container: ";
    for( auto x : ir3 ) {
        cout << x << " ";
    }

    return 0;
}