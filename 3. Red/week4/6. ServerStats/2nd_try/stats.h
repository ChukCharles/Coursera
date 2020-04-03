#pragma once

#include "http_request.h"

#include <string_view>
#include <algorithm>
#include <string>
#include <array>
#include <map>
using namespace std;


class Stats {
public:
    Stats() {
        for (int i = 0; i < _methods.size(); ++i){
            _m_stats[string_view(_methods[i])] = 0;
        }
        for (int i = 0; i < _uri.size(); ++i) {
            _u_stats[string_view(_uri[i])] = 0;
        }
    }
    void AddMethod(string_view method) {
        string test{method};
        for(int i = 0; i < _methods.size(); ++i) {
            if( test == _methods[i] ) {
                _m_stats[string_view(_methods[i])]++;
                return;
            }
        }
        _m_stats[string_view(_methods[4])]++;
        return;
    }
    void AddUri(string_view uri) {
        string test(uri);
        for( int i = 0; i < _uri.size(); ++i ) {
            if( test == _uri[i] ) {
                _u_stats[string_view(_uri[i])]++;
                return;
            }
        }
        _u_stats[string_view(_uri[5])]++;
        return;
    }
    const map<string_view, int>& GetMethodStats() const {
        return _m_stats;
    }
    const map<string_view, int>& GetUriStats() const {
        return _u_stats;
    }
private:
    map<string_view, int> _m_stats;
    map<string_view, int> _u_stats;

    array<string, 5> _methods{"PUT", "GET", "POST", "DELETE", "UNKNOWN"};
    array<string, 6> _uri{"/order", "/product", "/", "/basket", "/help", "unknown"};
};

HttpRequest ParseRequest(string_view line) {
    HttpRequest res;
    line.remove_prefix(std::min(line.find_first_not_of(" "), line.size()));
    size_t space = line.find(" ");
    res.method = line.substr(0, space);
    if( space == line.npos ) return res;
    line.remove_prefix(++space);
    space = line.find(" ");
    res.uri = line.substr(0, space);
    if( space == line.npos ) return res;
    line.remove_prefix(++space);
    res.protocol = line;
}
