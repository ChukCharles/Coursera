#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <sstream>
#include <future>
#include <functional>
using namespace std;


struct Stats {
    map<string, int> word_frequences;

    void operator += (const Stats& other) {
        for (auto x : other.word_frequences) {
            word_frequences[x.first] += x.second;
        }
    }
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
    stringstream ss(line);
    string word;
    Stats result;
    while (ss >> word) {
        if (key_words.count(word) > 0) result.word_frequences[word]++;
    }
    return result;
}

Stats ExploreKeyWordsSingleThread(
    const set<string>& key_words, vector<string> input
) {
    Stats result;
    for (string line : input) {
        //cout << line << " ";
        result += ExploreLine(key_words, line);
    }
    //for (auto p : result.word_frequences) cout << p.first << " " << p.second << endl;
    return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    const size_t max_batch_size = 5000;

    vector<string> batch;
    batch.reserve(max_batch_size);

    vector<future<Stats>> futures;

    for (string line; getline(input, line); ) {
        batch.push_back(move(line));
        if (batch.size() >= max_batch_size) {
            futures.push_back(
                async(ExploreKeyWordsSingleThread, ref(key_words), move(batch))
            );
            batch.reserve(max_batch_size);
        }
    }

    Stats result;

    if (!batch.empty()) {
        result += ExploreKeyWordsSingleThread(key_words, move(batch));
    }

    for (auto& f : futures) {
        result += f.get();
    }

    return result;

}

void TestBasic() {
    const set<string> key_words = { "yangle", "rocks", "sucks", "all" };

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    const auto stats = ExploreKeyWords(key_words, ss);
    const map<string, int> expected = {
      {"yangle", 6},
      {"rocks", 2},
      {"sucks", 1}
    };
    ASSERT_EQUAL(stats.word_frequences, expected);
}

/*void TestBasicSingle() {
    const set<string> key_words = { "yangle", "rocks", "sucks", "all" };

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    const auto stats = ExploreKeyWordsSingleThread(key_words, ss);
    const map<string, int> expected = {
      {"yangle", 6},
      {"rocks", 2},
      {"sucks", 1}
    };
    ASSERT_EQUAL(stats.word_frequences, expected);
}*/
int main() {
    TestRunner tr;
    RUN_TEST(tr, TestBasic);
    //RUN_TEST(tr, TestBasicSingle);
}
