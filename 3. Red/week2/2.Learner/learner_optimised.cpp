#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <set>

using namespace std;

class Learner {
 private:
  set<string> dict;

 public:
  int Learn(const vector<string>& words) {
    int newWords = 0;
    for (const auto& word : words) {
    	auto result = dict.insert(word);
		if (result.second) {
		++newWords;
		}
    }
    return newWords;
  }

  vector<string> KnownWords() {
	  vector<string> result(dict.begin(), dict.end());
	  return result;
  }
};
