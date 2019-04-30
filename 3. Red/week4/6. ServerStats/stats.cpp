#include "stats.h"

HttpRequest ParseRequest(string_view line) {
    vector<string_view> request_params;
    line.remove_prefix(std::min(line.find_first_not_of(" "), line.size()));
    while (true) {
        size_t space = line.find(' ');
        request_params.push_back(line.substr(0, space));
        if (space == line.npos) {
            break;
        } else {
            line.remove_prefix(space + 1);
        }
    }
    if( request_params.size() != 3 ) {
        //??
    }
    return { request_params[0], request_params[1], request_params[2] };
}

Stats::Stats() :
    m_stats({
        {"GET", 0},
        {"PUT", 0},
        {"POST", 0},
        {"DELETE", 0},
        {"UNKNOWN", 0},
    }),
    u_stats({
        {"/", 0},
        {"/order", 0},
        {"/product", 0},
        {"/basket", 0},
        {"/help", 0},
        {"unknown", 0}
    }) 
{}

void Stats::AddUri(string_view uri) {
    if( u_stats.count(uri) > 0 ) {
        u_stats[uri]++;
    } else {
        u_stats["unknown"]++;
    }
}

void Stats::AddMethod(string_view method) {
    if( m_stats.count(method) > 0 ) {
        m_stats[method]++;
    } else {
        m_stats["UNKNOWN"]++;
    }
}