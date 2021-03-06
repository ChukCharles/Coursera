#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

#include<map>
#include<deque>
#include<set>

using namespace std;

struct reserve {
    reserve( int64_t x, int y, int z)
        : time(x), cl_id(y), rooms_c(z)
    {}

    int64_t time;
    int cl_id;
    int rooms_c;
};

class BookingManager {
public:
    BookingManager()
        : last_time(0)
    {}

    void Book(int64_t time, string hotel, int client_id, int rooms) {
    	last_time = time;
    	reserve r( time, client_id, rooms );
    	orders[hotel].push_back(r);
        room_count[hotel] += rooms;
        cl_count[hotel][client_id]++;
    }

    double Clients(string hotel)
    {
    	if( orders.count(hotel) < 1 ) {
            return 0;
    	}
        Update(hotel);

        return cl_count[hotel].size();
    }
    int64_t Rooms(string hotel)
    {
    	if( orders.count(hotel) < 1 ) {
            return 0;
    	}
        Update(hotel);

        return room_count[hotel];
    }

private:
    void Update( string hotel ) {
        deque<reserve>& tmp = orders[hotel];        
        while( !tmp.empty() && tmp.front().time <= last_time-86400 ) {
            room_count[hotel] -= tmp.front().rooms_c;
            int& x = cl_count[hotel][tmp.front().cl_id];
            x--;
            if (x==0) {
                cl_count[hotel].erase(tmp.front().cl_id);
            }

            tmp.pop_front();
        }
    }
    
    int64_t last_time;
    map<string, deque<reserve>> orders;
    map<string, int> room_count;
    map<string, map<int, int>> cl_count;
};

int main() {
    //ios::sync_with_stdio(false);
    //cin.tie(nullptr);

    BookingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;

        if (query_type == "BOOK") {
            int64_t time;
            cin >> time;
            string hotel;
            cin >> hotel;
            int room_c;
            int client_id;
            cin >> client_id >> room_c;

            manager.Book(time, hotel, client_id, room_c);
        } else if (query_type == "ROOMS") {
            string hotel;
            cin >> hotel;
            cout << manager.Rooms(hotel) << "\n";
        } else if (query_type == "CLIENTS") {
            string hotel;
            cin >> hotel;
            cout << manager.Clients(hotel) << "\n";
        }
    }

    return 0;
}
