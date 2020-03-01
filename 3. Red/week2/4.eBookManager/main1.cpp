#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <fstream>

#include "profile.h"

using namespace std;

class ReadingManager {
public:

    ReadingManager()
    : user_page_counts_(MAX_USER_COUNT_ + 1, 0),
    sorted_users_(),
    user_positions_(MAX_USER_COUNT_ + 1, -1) {
    }

    void Read(int user_id, int page_count) {
        if (user_page_counts_[user_id] == 0) {
            AddUser(user_id);
        }
        user_page_counts_[user_id] = page_count;
        int& position = user_positions_[user_id];
        while (position > 0 && page_count > user_page_counts_[sorted_users_[position - 1]]) {
            SwapUsers(position, position - 1);
        }
    }

    double Cheer(int user_id) const {
        if (user_page_counts_[user_id] == 0) {
            return 0;
        }
        const int user_count = GetUserCount();
        if (user_count == 1) {
            return 1;
        }
        const int page_count = user_page_counts_[user_id];
        int position = user_positions_[user_id];
        while (position < user_count &&
                user_page_counts_[sorted_users_[position]] == page_count) {
            ++position;
        }
        if (position == user_count) {
            return 0;
        }
        // По умолчанию деление целочисленное, поэтому
        // нужно привести числитель к типу double.
        // Простой способ сделать это — умножить его на 1.0.
        return (user_count - position) * 1.0 / (user_count - 1);
    }

private:
    // Статическое поле не принадлежит какому-то конкретному
    // объекту класса. По сути это глобальная переменная,
    // в данном случае константная.
    // Будь она публичной, к ней можно было бы обратиться снаружи
    // следующим образом: ReadingManager::MAX_USER_COUNT.
    static const int MAX_USER_COUNT_ = 100'000;

    vector<int> user_page_counts_;
    vector<int> sorted_users_; // отсортированы по убыванию количества страниц
    vector<int> user_positions_; // позиции в векторе sorted_users_

    int GetUserCount() const {
        return sorted_users_.size();
    }

    void AddUser(int user_id) {
        sorted_users_.push_back(user_id);
        user_positions_[user_id] = sorted_users_.size() - 1;
    }

    void SwapUsers(int lhs_position, int rhs_position) {
        const int lhs_id = sorted_users_[lhs_position];
        const int rhs_id = sorted_users_[rhs_position];
        swap(sorted_users_[lhs_position], sorted_users_[rhs_position]);
        swap(user_positions_[lhs_id], user_positions_[rhs_id]);
    }
};

class ReadingManagerFast {
public:

    ReadingManagerFast()
    {
    }

    void Read(int user_id, int page_count) {
        int prev_page = 0;
        if (user_page_.count(user_id) > 0) {
            prev_page = user_page_[user_id];
            page_users_[prev_page]--;
        }
        user_page_[user_id] = page_count;
        page_users_[page_count]++;
    }

    double Cheer(int user_id) const {
        if( user_page_.count(user_id) == 0 ) return 0.0;
        if( user_page_.size() == 1 ) return 1.0;
        int p = user_page_.at(user_id);
        map<int, int>::const_iterator last = page_users_.lower_bound(p);
        map<int, int>::const_iterator cur = page_users_.begin();
        int count = 0.0;
        while ( cur != last ) {
            count += cur->second;
            cur = next(cur);
        }
        if (count == 0.0) return count;
        //cout << "cheer: " << count << " " << user_page_.size() << '\n';
        return count/(user_page_.size()*1.0-1);
    }

private:
    map<int, int> user_page_;
    map<int, int> page_users_;
};

class ReadingManagerFast2 {
public:

    ReadingManagerFast2()
    {
    }

    void Read(int user_id, int page_count) {
        int start_page = 1;
        if (user_page_.count(user_id) > 0) {
            start_page = user_page_[user_id]+1;
        }
        for( start_page; start_page <= page_count; start_page++ ) {
            page_users_[start_page]++;
        }
        user_page_[user_id] = page_count;
    }

    double Cheer(int user_id) const {
        if( user_page_.count(user_id) == 0 ) return 0.0;
        if( user_page_.size() == 1 ) return 1.0;
        int p = user_page_.at(user_id);
        map<int, int>::const_iterator faster_users = page_users_.lower_bound(p);
        int slower = user_page_.size() - faster_users->second;
        return slower/(user_page_.size()*1.0-1);
    }

private:
    map<int, int> user_page_;
    map<int, int> page_users_;
};
int main() {
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    // ios::sync_with_stdio(false);
    // cin.tie(nullptr);
    srand (time(NULL));

    ofstream out("eBook3.txt");
    int qcount = 1000000;
    out << qcount << "\n";
    
    for( int i = 0; i < qcount/2; i++ ) {
        out << "READ " << rand()%100 << " " << rand()%100 << "\n";
    }
    for (int i = qcount/2; i < qcount; i++ ) {
        out << "CHEER " << rand()%100 << '\n';
    }
    
    ifstream inp("eBook3.txt");

    ReadingManager manager1;
    ReadingManagerFast manager2;
    ReadingManagerFast2 manager3;

    /*{
        LOG_DURATION("slow manager");
        int query_count;
        inp >> query_count;

        for (int query_id = 0; query_id < query_count; ++query_id) {
            string query_type;
            inp >> query_type;
            int user_id;
            inp >> user_id;

            if (query_type == "READ") {
                int page_count;
                inp >> page_count;
                manager1.Read(user_id, page_count);
            } else if (query_type == "CHEER") {
                // cout << setprecision(6) << manager1.Cheer(user_id) << "\n";
                manager1.Cheer(user_id);
            }
        }
    }*/
    
    {
        LOG_DURATION("fast manager 1");
        int query_count;
        inp >> query_count;

        for (int query_id = 0; query_id < query_count; ++query_id) {
            string query_type;
            inp >> query_type;
            int user_id;
            inp >> user_id;

            if (query_type == "READ") {
                int page_count;
                inp >> page_count;
                manager2.Read(user_id, page_count);
            } else if (query_type == "CHEER") {
                // cout << setprecision(6) << manager2.Cheer(user_id) << "\n";
                manager2.Cheer(user_id);
            }
        }
    }
    
    {
        LOG_DURATION("fast manager 2");
        int query_count;
        inp >> query_count;

        for (int query_id = 0; query_id < query_count; ++query_id) {
            string query_type;
            inp >> query_type;
            int user_id;
            inp >> user_id;

            if (query_type == "READ") {
                int page_count;
                inp >> page_count;
                manager3.Read(user_id, page_count);
            } else if (query_type == "CHEER") {
                // cout << setprecision(6) << manager3.Cheer(user_id) << "\n";
                 manager3.Cheer(user_id);
            }
        }
    }
    /*srand (time(NULL));

    ofstream out("eBook2.txt");
    int qcount = 100000;
    out << qcount << "\n";
    
    for( int i = 0; i < qcount/2; i++ ) {
        out << "READ " << rand()%100 << " " << rand()%100 << "\n";
    }
    for (int i = qcount/2; i < qcount; i++ ) {
        out << "CHEER " << rand()%100 << '\n';
    }*/

    return 0;
}
