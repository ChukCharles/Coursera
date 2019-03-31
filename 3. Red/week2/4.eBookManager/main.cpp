#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

#include<map>
#include<set>
#include <fstream>
#include "profile.h"

using namespace std;

class ReadingManager {
public:

    ReadingManager() : pages_rating(MAX_PAGES_COUNT_+1, 0), curr_max_page_plus(0)
    {}
    //P-pages < 10^3
    //N-users < 10^5
    //Q-queries < 10^6

    void Read(int user_id, int page_count)  //O(logN) ~17
    {
        //curr_max_page_plus = max( curr_max_page_plus, page_count+1 );
        if( page_count+1 > curr_max_page_plus ) {
            int lv = pages_rating[curr_max_page_plus];
            for( int i = curr_max_page_plus+1; i <= page_count+1; ++i ) {
                pages_rating[i] = lv;
            }
            curr_max_page_plus = page_count+1;
        }
        if( users.count(user_id)>0 ) {
            int del_page = users[user_id];                          //O(logN)
            //pages[del_page].erase(user_id);    //O(logP + log(N))
            for( int i = del_page+1; i <= page_count; ++i) {
                pages_rating[i]--;
            }
        } else {
            for( int i = page_count+1; i < curr_max_page_plus+1; ++i ) {
                pages_rating[i]++;
            }
        }
        users[user_id] = page_count;
        
        /*for( int x : pages_rating ) {
            cout << x << " ";
        }
        cout << endl;*/
    }

    double Cheer(int user_id) const     //O(logN) ~10
    {
        double res = 0.0;
        if (users.count(user_id) < 1) {                     //O(logN)
            return res;
        }
        if (users.size() == 1) {
            return 1.0;
        }
        int p = users.at(user_id);                          //O(logN)
        res = pages_rating.at(p)*1.0 / (users.size()-1);    //O(logN)
        return res;
    }

private:
    // Статическое поле не принадлежит какому-то конкретному
    // объекту класса. По сути это глобальная переменная,
    // в данном случае константная.
    // Будь она публичной, к ней можно было бы обратиться снаружи
    // следующим образом: ReadingManager::MAX_USER_COUNT.
    static const int MAX_USER_COUNT_ = 100000;
    static const int MAX_PAGES_COUNT_ = 1000;

    //map<int, set<int>> pages;
    map<int, int> users;
    vector<int> pages_rating; //number of readers before this page
    int curr_max_page_plus;
};

int main() {
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        } else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }

    return 0;
}