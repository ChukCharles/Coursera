#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

#include<map>
#include<deque>
#include<set>

using namespace std;

struct reserve {
	reserve( double x, int64_t y, int z)
		: time(x), cl_id(y), rooms_c(z)
	{}

	double time;
	int64_t cl_id;
	int rooms_c;
};

class BookingManager {
public:

	BookingManager() : last_time(0) {}

    void Book(double time, string hotel, int64_t client_id, int rooms) {
    	last_time = time;
    	reserve r( time, client_id, rooms );
    	orders[hotel].push_back(r);
    }

    double Clients(string hotel)
    {
    	if( orders.count(hotel) < 1 ) {
    		return 0;
    	}
        deque<reserve>& tmp = orders[hotel];
        while( tmp.front().time <= last_time-86400 ) {
        	tmp.pop_front();
        }
        set<int64_t> unique_clients;
        for( auto& entry : tmp ) {
        	unique_clients.insert(entry.cl_id);
        }

        return unique_clients.size();
    }
    double Rooms(string hotel)
    {
    	double res = 0;
    	if( orders.count(hotel) < 1 ) {
    		return res;
    	}
        deque<reserve>& tmp = orders[hotel];
        while( tmp.front().time <= last_time-86400 ) {
        	tmp.pop_front();
        }
        for( auto& entry : tmp ) {
        	res += entry.rooms_c;
        }
        return res;
    }

private:
    double last_time;
    map<string, deque<reserve>> orders;
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
        	double time;
        	cin >> time;
        	string hotel;
        	cin >> hotel;
        	int room_c;
        	int64_t client_id;
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
