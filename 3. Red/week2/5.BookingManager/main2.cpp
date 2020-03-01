#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <deque>
#include <queue>
#include <set>
#include <fstream>

#include "profile.h"

using namespace std;

class HotelManagerTheir {
public:
  void Book(int64_t time, const string& hotel_name,
            int client_id, int room_count) {
    current_time_ = time;
    hotels_[hotel_name].Book({time, client_id, room_count});
  }
  int ComputeClientCount(const string& hotel_name) {
    return hotels_[hotel_name].ComputeClientCount(current_time_);
  }
  int ComputeRoomCount(const string& hotel_name) {
    return hotels_[hotel_name].ComputeRoomCount(current_time_);
  }

private:
  struct Booking {
    int64_t time;
    int client_id;
    int room_count;
  };

  class HotelInfo {
  public:
    void Book(const Booking& booking) {
      last_bookings_.push(booking);
      room_count_ += booking.room_count;
      ++client_booking_counts_[booking.client_id];
    }
    int ComputeClientCount(int64_t current_time) {
      RemoveOldBookings(current_time);
      return client_booking_counts_.size();
    }
    int ComputeRoomCount(int64_t current_time) {
      RemoveOldBookings(current_time);
      return room_count_;
    }
  private:
    static const int TIME_WINDOW_SIZE = 86400;
    queue<Booking> last_bookings_;
    int room_count_ = 0;
    map<int, int> client_booking_counts_;

    void PopBooking() {
      const Booking& booking = last_bookings_.front();
      room_count_ -= booking.room_count;
      const auto client_stat_it =
          client_booking_counts_.find(booking.client_id);
      if (--client_stat_it->second == 0) {
        client_booking_counts_.erase(client_stat_it);
      }
      last_bookings_.pop();
    }
    void RemoveOldBookings(int64_t current_time) {
      while (
          !last_bookings_.empty()
          && last_bookings_.front().time <= current_time - TIME_WINDOW_SIZE
      ) {
        PopBooking();
      }
    }
  };

  int64_t current_time_ = 0;
  map<string, HotelInfo> hotels_;
};

struct booking {
    long long time;
    int client;
    string hotel;
    int rooms;
};

class hotelManager {
public:
    hotelManager() {}
    
    void makeReservation( long long time, string hotel_name, int client_id, int room_count ) {
        _reservations.push_back({time, client_id, hotel_name, room_count});
        _hotel_rooms[hotel_name] += room_count;
        _hotel_clients[hotel_name].push_back(client_id);
        //deleting old data
        time -= _day_seconds;
        booking& first = _reservations.front();
        while( !_reservations.empty() &&
                first.time <= time ) {
            _hotel_rooms[first.hotel] -= first.rooms;
            _hotel_clients[first.hotel].pop_front();
            _reservations.pop_front();
            first = _reservations.front();
        }
    }
    
    int Rooms(string hotel_name) {
        return _hotel_rooms[hotel_name];
    }
    
    int Clients(string hotel_name) {
        set<int> unique_clients;
        for( int& cl : _hotel_clients[hotel_name]) {
            unique_clients.insert(cl);
        }
        return unique_clients.size();
    }

private:
    deque<booking> _reservations;
    map<string, int> _hotel_rooms;
    map<string, deque<int>> _hotel_clients;
    long long _last_book_time;
    const int _day_seconds = 86400;
};

int main() {
    srand (time(NULL));

    ofstream out("hotelBooking2.txt");
    int qcount = 99999;
    out << qcount << "\n";
    long long time = 0;
    for( int i = 0; i < qcount/3; i++ ) {
        out << "BOOK "  << time + rand()%100000 << " hotel" << rand()%100 << " " << rand()%100 <<  " " << rand()%100 << "\n";
    }
    for (int i = qcount/3; i < qcount*2/3; i++ ) {
        out << "ROOMS " << rand()%100 << '\n';
    }
    for (int i = qcount*2/3; i < qcount; i++ ) {
        out << "CLIENTS " << rand()%100 << '\n';
    }
    
    

    //ifstream inp("hotelBooking1.txt");

    hotelManager manager1;
    HotelManagerTheir manager2;

    {
        LOG_DURATION("hotel manager");
        ifstream inp("hotelBooking2.txt");
        int query_count;
        inp >> query_count;

        for (int query_id = 0; query_id < query_count; ++query_id) {
            string query_type;
            inp >> query_type;

            if (query_type == "BOOK") {
                long long time;
                string hotelName;
                int client, rooms;
                inp >> time >> hotelName >> client >> rooms;
                manager1.makeReservation(time, hotelName, client, rooms);
            } else if (query_type == "CLIENTS") {
                string hotelName;
                inp >> hotelName;
                //cout <<  manager1.Clients(hotelName) << "\n";
                manager1.Clients(hotelName);
            } else if (query_type == "ROOMS") {
                string hotelName;
                inp >> hotelName;
                //cout << manager1.Rooms(hotelName) << "\n";
                manager1.Rooms(hotelName);
            }
        }
    }
    
    {
        LOG_DURATION("hotel manager ethalon");
        ifstream inp2("hotelBooking2.txt");
        int query_count;
        inp2 >> query_count;

        for (int query_id = 0; query_id < query_count; ++query_id) {
            string query_type;
            inp2 >> query_type;

            if (query_type == "BOOK") {
                int64_t time;
                string hotelName;
                int client, rooms;
                inp2 >> time >> hotelName >> client >> rooms;
                manager2.Book(time, hotelName, client, rooms);
            } else if (query_type == "CLIENTS") {
                string hotelName;
                inp2 >> hotelName;
                //cout <<  manager2.Clients(hotelName) << "\n";
                manager2.ComputeClientCount(hotelName);
            } else if (query_type == "ROOMS") {
                string hotelName;
                inp2 >> hotelName;
                //cout << manager2.Rooms(hotelName) << "\n";
                manager2.ComputeRoomCount(hotelName);
            }
        }
    }

    return 0;
}
