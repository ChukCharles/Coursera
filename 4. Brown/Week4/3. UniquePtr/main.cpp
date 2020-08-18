#include "test_runner.h"

#include <cstddef>  // нужно для nullptr_t

using namespace std;

// Реализуйте шаблон класса UniquePtr
template <typename T>
class UniquePtr {
private:
    T* pointer_ = nullptr;
public:
    UniquePtr() : pointer_(nullptr) {}
    UniquePtr(T* ptr) : pointer_(ptr) {}
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr(UniquePtr&& other) {
        if (pointer_) delete pointer_;
        pointer_ = other.Release();
    };
    UniquePtr& operator = (const UniquePtr&) = delete;
    UniquePtr& operator = (nullptr_t) {
        if (pointer_) delete pointer_;
        pointer_ = nullptr;
        return *this;
    }
    UniquePtr& operator = (UniquePtr&& other) {
        if (pointer_) delete pointer_;
        pointer_ = other.Release();
        return *this;
    }
    ~UniquePtr() {
        delete pointer_;
    }

    T& operator * () const {
        return *pointer_;
    }

    T* operator -> () const {
        return pointer_;
    }

    T* Release() {
        T* result = pointer_;
        pointer_ = nullptr;
        return result;
    }

    void Reset(T* ptr) {
        if (pointer_) delete pointer_;
        pointer_ = ptr;
    }

    void Swap(UniquePtr& other) {
        T* tmp = other.Release();
        other.Reset(pointer_);
        pointer_ = tmp;
    }

    T* Get() const {
        return pointer_;
    }
};


struct Item {
    static int counter;
    int value;
    Item(int v = 0) : value(v) {
        ++counter;
    }
    Item(const Item& other) : value(other.value) {
        ++counter;
    }
    ~Item() {
        --counter;
    }
};

int Item::counter = 0;


void TestLifetime() {
    Item::counter = 0;
    {
        UniquePtr<Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        ptr.Reset(new Item);
        ASSERT_EQUAL(Item::counter, 1);
    }
    ASSERT_EQUAL(Item::counter, 0);

    {
        UniquePtr<Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        auto rawPtr = ptr.Release();
        ASSERT_EQUAL(Item::counter, 1);

        delete rawPtr;
        ASSERT_EQUAL(Item::counter, 0);
    }
    ASSERT_EQUAL(Item::counter, 0);
}

void TestGetters() {
    UniquePtr<Item> ptr(new Item(42));
    ASSERT_EQUAL(ptr.Get()->value, 42);
    ASSERT_EQUAL((*ptr).value, 42);
    ASSERT_EQUAL(ptr->value, 42);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestLifetime);
    RUN_TEST(tr, TestGetters);
}
