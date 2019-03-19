#include <vector>
#include <set>
#include <iostream>
#include <algorithm>

using namespace std;

vector<string> SplitIntoWords(const string& s) {
	vector<string> res(0);
	string tmp = "";
	for( string::const_iterator it = s.begin(); it != s.end(); it++ ) {
		if( *it == ' ' ) {
			res.push_back(tmp);
			tmp = "";
		} else {
			tmp += *it;
		}
	}
	if( tmp.length() > 0 ) {
		res.push_back( tmp );
	}
	return res;
}
/*alternative:
vector<string> SplitIntoWords(const string& str) {
  // Готовим вектор, в который будем сохранять слова
  vector<string> result;

  // Эти два итератора будут ограничивать текущую не разбитую часть строки
  // str_begin будет двигаться вправо
  auto str_begin = begin(str);
  // str_end всегда будет указывать на конец строки (поэтому он константный)
  const auto str_end = end(str);

  // У цикла нет условия завершения, так что единственный способ закончить его —
  // сделать break
  while (true) {
    
    // Находим первый пробел в текущем «хвосте» строки
    auto it = find(str_begin, str_end, ' ');
    
    // Полуинтервал [str_begin, it) — очередное слово
    result.push_back(string(str_begin, it));
    
    if (it == str_end) {
      // Если пробелов больше нет, цикл пора завершить.
      // Последнее слово уже добавлено
      break;
    } else {
      // Иначе первый символ после пробела — начало очередного слова.
      // Туда и подвинем str_begin
      str_begin = it + 1;
    }
    
  }

  return result;
}
*/

int main() {
  string s = "C Cpp Java Python";

  vector<string> words = SplitIntoWords(s);
  cout << words.size() << " ";
  for (auto it = begin(words); it != end(words); ++it) {
    if (it != begin(words)) {
      cout << "/";
    }
    cout << *it;
  }
  cout << endl;

  return 0;
}

