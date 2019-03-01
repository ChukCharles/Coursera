#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <functional>
#include <stdexcept>
#include <algorithm>
#include "date.h"

using namespace std;

class Database {
public:
	Database() {}

	void Add(const Date date, string event);
	ostream& Print( ostream& os ) const;
	int RemoveIf(function<bool(const Date& date, const string& event)> pred);
	vector<string> FindIf(function<bool(const Date& date, const string& event)> pred) const;
	string Last( const Date& d ) const;

private:
	map<Date, set<string>> storage;
	map<Date, vector<string>> p_storage;
};
