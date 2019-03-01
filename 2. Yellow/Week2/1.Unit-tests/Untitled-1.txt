#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator>>(istream& is, Query& q) {
    q.stops.clear();
    q.bus = "";
    q.stop = "";
    string qt;
    is >> qt;

    if ( qt == "NEW_BUS" ) {
        q.type = QueryType::NewBus;
        is >> q.bus;
        int stop_count;
        is >> stop_count;
        for ( int i=0; i<stop_count; ++i ) 
        {
            string stop; 
            is >> stop;
            q.stops.push_back(stop);
        }
    } else if( qt == "BUSES_FOR_STOP" ) {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    } else if( qt == "STOPS_FOR_BUS" ) {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    } else if( qt == "ALL_BUSES" ) {
        q.type = QueryType::AllBuses;
    }
    return is;
}

struct BusesForStopResponse {
    string stop;
    vector<string> buses;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    if( r.buses.size() == 0 ) {
        os << "No stop";
        return os;
    }
    os << "Stop " << r.stop << ": ";
    for( int i = 0; i < r.buses.size(); ++i ) {
        os << r.buses[i];
        if( i != r.buses.size()-1 ) {
            os << " ";
        }
    }
    //os << endl;
    return os;
}

struct StopsForBusResponse {
    string bus;
    vector<string, vector<string>> stops;
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    if( r.stops.size() == 0 ) {
        os << "No bus";
        return os;
    }

    for( auto s : r.stops ) {
        os << "Stop " << s.first << ": ";
        if( s.second.size() == 1 ) {
            os << "No intercharge\n";
            continue;
        }
        for( int i = 0; i < s.second.size(); ++i ) {
            if( s.second[i] != r.bus ) {
                os << s.second[i] << " ";
            }
        }
        os << endl;
    }

    return os;
}

struct AllBusesResponse {
    map<string, vector<string>> bts;
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    if( r.bts.size() == 0 ) {
        os << "No buses";
        return os;
    }
    for( auto m : r.bts ) {
        os << "Bus " << m.first << ": ";
        for( int i = 0; i < m.second.size(); ++i ) {
            os << m.second[i];
            if( i != m.second.size()-1 ) {
                os << " ";
            }
        }
        os << endl;
    }
    return os;
}

class BusManager {
public:
    BusManager() {
        buses_to_stops.clear();
        stops_to_buses.clear();
    }

    void AddBus( const string& bus, const vector<string>& stops ) {
        buses_to_stops[bus] = stops;
        for( string s : stops ) {
            stops_to_buses[s].push_back( bus );
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        BusesForStopResponse result;
        result.stop = stop;
        if( stops_to_buses.count( stop ) == 0 ) {
            result.buses.resize(0);
        } else {
            result.buses = stops_to_buses.at(stop);
        }
        return result;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        StopsForBusResponse result;
        result.bus = bus;
        if ( buses_to_stops.count( bus ) == 0 ) {
            result.stops.clear();
        } else {
            vector<string> ss = buses_to_stops.at(bus);
            for( string stop : ss ) {
                result.stops[stop] = stops_to_buses.at(stop);
            }
        }
        return result;
    }

    AllBusesResponse GetAllBuses() const {
        AllBusesResponse result;
        result.bts = buses_to_stops;
        return result;
    }
private:
    map<string, vector<string>> buses_to_stops;
    map<string, vector<string>> stops_to_buses;
};

// Не меняя тела функции main, реализуйте функции и классы выше

int main() {
    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
        case QueryType::NewBus:
            bm.AddBus(q.bus, q.stops);
            break;
        case QueryType::BusesForStop:
            cout << bm.GetBusesForStop(q.stop) << endl;
            break;
        case QueryType::StopsForBus:
            cout << bm.GetStopsForBus(q.bus) << endl;
            break;
        case QueryType::AllBuses:
            cout << bm.GetAllBuses() << endl;
            break;
        }
    }

    return 0;
}
