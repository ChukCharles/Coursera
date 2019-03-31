#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

#include<map>
#include<queue>
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
    	reserve r( time, client_id, rooms );
    	orders[hotel].push(r);
        
        last_time = time;
        u_clients[hotel].insert(client_id);
        rooms_count[hotel] += rooms;
    }

    double Clients(string hotel)
    {
    	if( orders.count(hotel) < 1 ) {
            return 0;
    	}
        Update(hotel);
        return u_clients[hotel].size();
    }
    int64_t Rooms(string hotel)
    {
    	if( orders.count(hotel) < 1 ) {
            return 0;
    	}
        Update(hotel);
        return rooms_count[hotel];
    }

private:
    void Update(string hotel) {
        queue<reserve>& tmp = orders[hotel];
        while( !tmp.empty() && tmp.front().time <= last_time-86400 ) {
            u_clients[hotel].erase(tmp.front().cl_id);
            rooms_count[hotel] -= tmp.front().rooms_c;
            tmp.pop();
        }
    }
    
    int64_t last_time;
    map<string, queue<reserve>> orders;
    map<string, set<int>> u_clients;
    map<string, uint64_t> rooms_count;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

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
