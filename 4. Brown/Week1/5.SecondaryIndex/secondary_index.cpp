#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <list>
#include <iterator>
#include <unordered_map>
#include <unordered_set>

#include <utility>

using namespace std;

struct Record {
    string id;
    string title;
    string user;
    int timestamp;
    int karma;

    bool operator==(const Record& other) const {
        return (id == other.id &&
                title == other.title &&
                user == other.user &&
                timestamp == other.timestamp &&
                karma == other.karma);
    }
};
ostream& operator<<(ostream& os, const Record& r) {
    return os << '[' << r.id << ' ' << r.title << ' ' << r.user
        << ' ' << r.timestamp << ' ' << r.karma << ']';
}

struct RecordHasher {
    hash<string> shash;
    hash<int> ihash;

    size_t operator() (const Record& rec) const {
        size_t idH = shash(rec.id);
        size_t titleH = shash(rec.title);
        size_t userH = shash(rec.user);
        size_t tsH = ihash(rec.timestamp);
        size_t karmaH = ihash(rec.karma);

        int n = 13;
        size_t result = idH + titleH*n*n + userH*n*n*n + 
                        tsH*n*n*n*n + karmaH*n*n*n*n*n;
        return result;
    }
};

class RecordIteratorHasher {
public:
    size_t operator()(const list<Record>::iterator& rit) const {
        hash<string> shash;
        return shash(rit->id);
    }
};

using RecordIt = list<Record>::iterator;

//template <class K, class V>
//void PrintItSet(const map<K, unordered_set<V>>& m) {
//    for (auto item : m) {
//        for (auto iter : item.second) cout << *item << endl;
//    }
//}
// Реализуйте этот класс
class Database {
public:
    bool Put(const Record& record) {
        if (id_to_rec_it.count(record.id) > 0) return false;

        records.push_back(record);
        RecordIt rit = prev(records.end());

        id_to_rec_it[record.id] = rit;
        karma_to_rec_it[record.karma].insert(rit);
        time_to_rec_it[record.timestamp].insert(rit);
        user_to_rec_it[record.user].insert(rit);

        return true;
    }
    const Record* GetById(const string& id) const {
        if (id_to_rec_it.count(id) == 0) return nullptr;

        Record* rit = &(*id_to_rec_it.at(id));
        return rit;
    }
    bool Erase(const string& id) {
        if (id_to_rec_it.count(id) == 0) return false;

        RecordIt rit = id_to_rec_it[id];
        auto sr = user_to_rec_it[rit->user].find(rit);
        if (sr != user_to_rec_it[rit->user].end()) user_to_rec_it[rit->user].erase(sr);
        //PrintItSet<string, RecordIt>(user_to_rec_it);

        sr = time_to_rec_it[rit->timestamp].find(rit);
        if (sr != time_to_rec_it[rit->timestamp].end()) time_to_rec_it[rit->timestamp].erase(sr);

        sr = karma_to_rec_it[rit->karma].find(rit);
        if (sr != karma_to_rec_it[rit->karma].end()) karma_to_rec_it[rit->karma].erase(sr);

        records.erase(rit);
        id_to_rec_it.erase(id);
        return true;
    }

    template <typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const;

    template <typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const;

    template <typename Callback>
    void AllByUser(const string& user, Callback callback) const;

    void LogRecords() {
        for (auto rec : records) cout << rec << endl;
    }
private:
    list<Record> records;
    unordered_map<string, RecordIt> id_to_rec_it;
    map< int, unordered_set< RecordIt, RecordIteratorHasher >> karma_to_rec_it;
    map< int, unordered_set< RecordIt, RecordIteratorHasher >> time_to_rec_it;
    map< string, unordered_set< RecordIt, RecordIteratorHasher >> user_to_rec_it;
};

template <typename Callback>
void Database::RangeByTimestamp(int low, int high, Callback callback) const {
    while (true) {
        auto it = time_to_rec_it.lower_bound(low);
        if (it == time_to_rec_it.end() || it->first > high) return;
        for (auto rit : it->second) {
            if (!callback(*rit)) return;
        }
        low = it->first + 1;
    }
}

template <typename Callback>
void Database::RangeByKarma(int low, int high, Callback callback) const {
    while (true) {
        auto it = karma_to_rec_it.lower_bound(low);
        if (it == karma_to_rec_it.end() || it->first > high) return;
        for (auto rit : it->second) {
            if (!callback(*rit)) return;
        }
        low = it->first + 1;
    }
}

template <typename Callback>
void Database::AllByUser(const string& user, Callback callback) const {
    if (user_to_rec_it.count(user) == 0) return;
    for (auto rit : user_to_rec_it.at(user)) {
        if (!callback(*rit)) return;
    }
}
void TestRangeBoundaries() {
    const int good_karma = 1000;
    const int bad_karma = -10;

    Database db;
    db.Put({ "id1", "Hello there", "master", 1536107260, good_karma });
    db.Put({ "id2", "O>>-<", "general2", 1536107260, bad_karma });

    int count = 0;
    db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
        ++count;
        return true;
        });

    ASSERT_EQUAL(2, count);
}

void TestSameUser() {
    Database db;
    db.Put({ "id1", "Don't sell", "master", 1536107260, 1000 });
    db.Put({ "id2", "Rethink life", "master", 1536107260, 2000 });

    int count = 0;
    db.AllByUser("master", [&count](const Record&) {
        ++count;
        return true;
        });

    ASSERT_EQUAL(2, count);
}

void TestReplacement() {
    const string final_body = "Feeling sad";

    Database db;
    db.Put({ "id", "Have a hand", "not-master", 1536107260, 10 });
    db.Erase("id");
    db.Put({ "id", final_body, "not-master", 1536107260, -10 });

    auto record = db.GetById("id");
    ASSERT(record != nullptr);
    ASSERT_EQUAL(final_body, record->title);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestRangeBoundaries);
    RUN_TEST(tr, TestSameUser);
    RUN_TEST(tr, TestReplacement);

    //Database db;
    //db.Put({ "id1", "O>>-<", "general1", 10, -100 });
    //db.Put({ "id2", "O>>-<", "general2", 5, 5 });
    //db.Put({ "id3", "O>>-<", "general2", 27, -100 });
    //db.Put({ "id4", "O>>-<", "general2", 27, 20 });
    ////db.Erase("id2");

    //db.RangeByKarma(-100, 100, [](const Record& r) {
    //    cout << r << endl; return true; });

    return 0;
}
