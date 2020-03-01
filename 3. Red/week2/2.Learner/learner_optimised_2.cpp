
#include "profile.h"
#include "test_runner.h"

#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

class Learner {
private:
    set<string> sorted_dict;

public:
    int Learn(const vector<string>& words) {
        int newWords = 0;
        for (const auto& word : words) {
            if (sorted_dict.find(word) == sorted_dict.end()) {
                ++newWords;
                sorted_dict.insert(word);
            }
        }
        return newWords;
    }

    vector<string> KnownWords() {
        return {sorted_dict.begin(), sorted_dict.end()};
    }
};

vector<vector<string>> readBook(string name) {
    ifstream input(name);
    string tmp;
    vector<vector<string>> text;
    
    while (getline(input, tmp)) {
        if(tmp.size() <= 1) continue;
        vector<string> line;
        stringstream tmps(tmp);
        string word;
        while(tmps >> word) {
            line.push_back(word);
        }
        //cout << line.size() << endl;
        text.push_back(line);
    }
    return text;
}

int main() {
    
    vector<vector<string>> book1 = readBook("Tartt Donna. The Secret History.txt");
    cout << book1.size();

    Learner slowLearner;
    {
        LOG_DURATION("Slow Learner::Learn");
        
        for( auto& pack : book1 ) {
            slowLearner.Learn(pack);
            //cout << "new words learned: " << slowLearner.Learn(pack) << endl;
        }
    }

    //vector<vector<string>> book2 = readBook("bishop_briggs.txt");
    {
        LOG_DURATION("Slow Learner::KnownWords");
        slowLearner.KnownWords();
    }
    return 0;
}