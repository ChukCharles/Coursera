#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>

#include <stdlib.h>
#include <time.h>
#include <set>
#include "profile.h"

using namespace std;

class RouteManager {
public:
  void AddRoute(int start, int finish) {
    reachable_lists_[start].push_back(finish);
    reachable_lists_[finish].push_back(start);
  }
  int FindNearestFinish(int start, int finish) const {
    int result = abs(start - finish);
    if (reachable_lists_.count(start) < 1) {
        return result;
    }
    const vector<int>& reachable_stations = reachable_lists_.at(start);
    if (!reachable_stations.empty()) {
      result = min(
          result,
          abs(finish - *min_element(
              begin(reachable_stations), end(reachable_stations),
              [finish](int lhs, int rhs) { return abs(lhs - finish) < abs(rhs - finish); }
          ))
      );
    }
    return result;
  }
private:
  map<int, vector<int>> reachable_lists_;
};

class RouteManagerFast {
public:
  void AddRoute(int start, int finish) {
    reachable_lists_[start].insert(finish);
    reachable_lists_[finish].insert(start);
  }
  int FindNearestFinish(int start, int finish) const {
    int result = abs(start - finish);
    if (reachable_lists_.count(start) < 1) {
        return result;
    }
    
    auto lb = reachable_lists_.at(start).lower_bound(finish);
    if ( lb == reachable_lists_.at(start).end() ) {
        return min(abs( *(prev(lb)) - finish ), result);
    }
    if ( *lb == finish ) return 0;
    int l1 = abs( *lb - finish );
    int l2 = abs( *(prev(lb)) - finish );
    result = min(min( l1, l2 ), result);
    return result;
  }
private:
  map<int, set<int>> reachable_lists_;
};

int main() {
    ifstream inp("express2.txt");
    RouteManager routes;
    int query_count;
    inp >> query_count;
    {
        LOG_DURATION("express slow");
        for (int query_id = 0; query_id < query_count; ++query_id) {
            string query_type;
            inp >> query_type;
            int start, finish;
            inp >> start >> finish;
            if (query_type == "ADD") {
                routes.AddRoute(start, finish);
            } else if (query_type == "GO") {
                //cout << routes.FindNearestFinish(start, finish) << "\n";
                routes.FindNearestFinish(start, finish);
            }
        }
    }
    
    ifstream inp2("express2.txt");
    RouteManagerFast routesFast;

    query_count;
    inp2 >> query_count;
    {
        LOG_DURATION("express slow");
        for (int query_id = 0; query_id < query_count; ++query_id) {
            string query_type;
            inp2 >> query_type;
            int start, finish;
            inp2 >> start >> finish;
            if (query_type == "ADD") {
                routesFast.AddRoute(start, finish);
            } else if (query_type == "GO") {
                //cout << routes.FindNearestFinish(start, finish) << "\n";
                routesFast.FindNearestFinish(start, finish);
            }
        }
    }
    /*srand (time(NULL));

    ofstream out("express2.txt");
    int qcount = 100000;
    out << qcount << "\n";
    
    for( int i = 0; i < qcount/2; i++ ) {
        out << "ADD " << rand()%100 << " " << rand()%100 << "\n";
    }
    for (int i = qcount/2; i < qcount; i++ ) {
        out << "GO " << rand()%100 << " " << rand()%100 << '\n';
    }*/

    return 0;
}
