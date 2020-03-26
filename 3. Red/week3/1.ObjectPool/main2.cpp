#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:

    T* Allocate() {
        if (_deactivated.size() > 0) {
            T* tmp = _deactivated.front();
            _deactivated.pop();
            _active.insert(tmp);
            return tmp;
        }
        T* element = new T();
        _active.insert(element);
        return element;
    }
    T* TryAllocate() {
        if (_deactivated.size() > 0) {
            T* tmp = _deactivated.front();
            _deactivated.pop();
            _active.insert(tmp);
            return tmp;
        } else {
            return nullptr;
        }
    }

    void Deallocate(T* object) {
        if (_active.count(object) < 1) {
            throw(std::invalid_argument(""));
        } else {
            _deactivated.push(object);
            _active.erase(object);
        }
    }

    ~ObjectPool() {
        for ( auto i : _active ) {
            delete i;
        }
        
        while( _deactivated.size() > 0 ) {
            auto i = _deactivated.front();
            delete i;
            _deactivated.pop();
        }
    }

private:
    set<T*> _active;
    queue<T*> _deactivated;
};

void TestObjectPool() {
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    ASSERT_EQUAL(*pool.Allocate(), "second");

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third");
    ASSERT_EQUAL(*pool.Allocate(), "first");

    pool.Deallocate(p1);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);

    return 0;
}
