#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>

using namespace std;

template<class T>
class ObjectPool {
public:
	T* Allocate() {
		T* result;
		if( !released.empty() ) {
			result = released.front();
			released.pop();
		} else {
			result = new T;
		}
		active.insert( result );
		return result;
	}
	T* TryAllocate() {
		T* result;
		if( !released.empty() ) {
			result = released.front();
			active.insert( result );
			released.pop();
		} else {
			result = nullptr;
		}
		return result;
	}

	void Deallocate(T* object) {
		if( active.count( object ) < 1 ) {
			throw invalid_argument("");
		} else {
			T* tmp = *active.find( object );
			released.push( tmp );
			active.erase( tmp );
		}
	}

	~ObjectPool() {
		for( auto x : active ) {
			delete x;
		}
		while( !released.empty() ) {
			delete released.front();
			released.pop();
		}
	}

private:
	// Добавьте сюда поля
	set<T*> active;
	queue<T*> released;
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
