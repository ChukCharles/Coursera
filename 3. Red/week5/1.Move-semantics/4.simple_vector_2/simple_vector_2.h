//#pragma once

#include <cstdlib>
#include <algorithm>
#include <iostream>

using namespace std;

// Реализуйте шаблон SimpleVector

template <typename T>
class SimpleVector {
public:

    SimpleVector()
        : data(nullptr), sz_(0), cap_(0)
    {}

    explicit SimpleVector(size_t size)
        : sz_(size), cap_(size), data(new T[size])
    {}

    ~SimpleVector() {
        delete[] data;
    }

    T& operator[](size_t index) { return data[index]; }
    T* begin() const { return data; }
    T* end() const { return data + sz_; }

    size_t Size() const { return sz_; }
    size_t Capacity() const { return cap_; }

    void PushBack(T value) {
        if( cap_ == 0 ) {
            data = new T[1];
            data[0] = move(value);
            cap_ = 1;
            sz_ = 1;
        } else if ( cap_ == sz_ ) {
            T* tmp = new T[cap_*2];
            for( int i = 0; i < sz_; ++i ) {
                tmp[i] = move(data[i]);
            }
            tmp[sz_] = move(value);
            delete[] data;
            data = tmp;
            sz_++;
            cap_ *= 2;
        } else {
            data[sz_] = move(value);
            sz_++;
        }
    }

private:
    // Добавьте поля для хранения данных вектора
    size_t sz_;
    size_t cap_;
    T* data;
};