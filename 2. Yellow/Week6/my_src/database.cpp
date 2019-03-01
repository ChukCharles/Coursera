#include "database.h"

void Database::Add(const Date date, string event) {
	if(storage[date].count(event) == 0) {
		p_storage[date].push_back(event);
	}
	storage[date].insert(event);
}

ostream& Database::Print( ostream& os ) const {
    for (const auto& item : p_storage) {
      for (const string& event : item.second) {
    	  os << item.first << " " << event << endl;
      }
    }
    return os;
}

int Database::RemoveIf(function<bool(const Date& date, const string& event)> pred) {
	map<Date, vector<set<string>::iterator>> del;
	int count = 0;

	for (auto& item : storage) {
		for (auto it = item.second.begin(); it != item.second.end(); ++it) {
			if (pred(item.first, *it)) {
				del[item.first].push_back(it);
				++count;
			}
		}
	}
	for (auto item : del) {
		for (auto event_it : item.second) {
			//deleting from storage
			storage[item.first].erase(event_it);
			if (storage[item.first].size()==0) {
				storage.erase(item.first);
				//cout << "date deleted: " << item.first << endl;
			}
			//deleting from print list:
			auto del_it = find(p_storage[item.first].begin(), p_storage[item.first].end(), *event_it);
			p_storage[item.first].erase(del_it);
			if (p_storage[item.first].size()==0) {
				p_storage.erase(item.first);
			}
		}
	}

	return count;
}

vector<string> Database::FindIf(function<bool(const Date& date, const string& event)> pred) const {
	vector<string> result;
	for (auto& item : p_storage) {
		for (auto it = item.second.begin(); it != item.second.end(); ++it) {
			if (pred(item.first, *it)) {
				stringstream tmp;
				tmp << item.first << " " << *it;
				result.push_back(tmp.str());
			}
		}
	}
	return result;
}

string Database::Last( const Date& d ) const {
	stringstream res;
	auto it = p_storage.upper_bound(d);
	if (it == p_storage.begin()) {
		throw invalid_argument("");
	}
	--it;
	res << it->first << " " << *(--(it->second).end());
	return res.str();
}
