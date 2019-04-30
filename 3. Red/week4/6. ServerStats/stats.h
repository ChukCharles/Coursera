#pragma once

#include "http_request.h"

#include <string_view>
#include <map>
#include <vector>

using namespace std;

HttpRequest ParseRequest(string_view line);

class Stats {
public:
    Stats();
    
    void AddMethod(string_view method);
    void AddUri(string_view uri);

    const map<string_view, int>& GetMethodStats() const {
        return m_stats;
    }
    const map<string_view, int>& GetUriStats() const {
        return u_stats;
    }

private:
    map<string_view, int> m_stats;
    map<string_view, int> u_stats;
};
