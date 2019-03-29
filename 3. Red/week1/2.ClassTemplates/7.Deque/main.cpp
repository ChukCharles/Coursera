#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <stdexcept>
using namespace std;

template<typename T>
class Deque {
public:
    Deque() {
        front_.resize(0);
        back_.resize(0);
    }
    bool Empty() const { return (back_.size() + front_.size()) == 0; }
    size_t Size() const { return back_.size() + front_.size(); }
    T& operator[](size_t index) {
        if( index >= front_.size() ) {
            return back_[index-front_.size()];
        } else {
            return front_[front_.size()-index-1];
        }
    }
    const T& operator[](size_t index) const {
        if( index >= front_.size() ) {
            return back_[index-front_.size()];
        } else {
            return front_[front_.size()-index-1];
        }
    }
    
    T& At(size_t index) {
        if( index >= Size() ) {
            throw out_of_range("oops");
        }
        if( index >= front_.size() ) {
            return back_.at(index-front_.size());
        } else {
            return front_.at(front_.size()-index-1);
        }
    }
    const T At(size_t index) const {
        if( index >= Size() ) {
            throw out_of_range("oops");
        }
        if( index >= front_.size() ) {
            return back_.at(index-front_.size());
        } else {
            return front_.at(front_.size()-index-1);
        }
    }
    
    T& Front() {
        if( front_.size() > 0 ) {
            return front_[front_.size()-1];
        } else if( back_.size() > 0 ) {
            return back_[0];
        } else {
            return *front_.end();
        }
    }
    const T& Front() const { 
        if( front_.size() > 0 ) {
            return front_.at(front_.size()-1);
        } else if( back_.size() > 0 ) {
            return back_[0];
        } else {
            return *front_.end();
        }
    }
    T& Back() {
        if( back_.size() > 0 ) {
            return back_[back_.size()-1];
        } else if( front_.size() > 0 ) {
            return front_[0];
        } else {
            return *back_.end();
        }
    }
    const T& Back() const {
        if( back_.size() > 0 ) {
            return back_.at(back_.size()-1);
        } else if( front_.size() > 0 ) {
            return front_[0];
        } else {
            return *back_.end();
        }
    }
    void PushFront( T value ) {
        front_.push_back(value);
    }
    void PushBack( T value ) {
        back_.push_back( value );
    }
    void Print() {
        for( T x : front_ ) {
            cout << x << " ";
        }
        cout << endl;
        for( T x : back_) {
            cout << x << " ";
        }
        cout << endl;
    }
    private:
    vector<T> front_;
    vector<T> back_;
};

int main() {
    try {
        Deque<int> dq1, dq2;
        dq1.PushBack(3);
        dq1.PushBack(4);
        dq1.PushFront(2);
        dq1.PushFront(1);
        dq1.Print();
        cout << dq1[0] << " " << dq1[1] << " " << dq1[2] << " " << dq1[3] << endl;
        //cout << dq1[0] << endl;
        deque<int> std_deq;
        std_deq.push_front(1);
        std_deq.push_front(2);
        cout << std_deq.front() << " " << std_deq.back() << endl;

    } catch ( exception& ex ) {
        cout << ex.what();
    }
  
  return 0;
}
