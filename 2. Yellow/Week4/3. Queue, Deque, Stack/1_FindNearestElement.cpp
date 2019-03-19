#include <vector>
#include <set>
#include <iterator>
#include <iostream>
#include <algorithm>

using namespace std;

set<int>::const_iterator FindNearestElement(const set<int>& numbers,
                                            int border) {

  const auto first_not_less = numbers.lower_bound(border);

  if (first_not_less == numbers.begin()) {
    return first_not_less;
  }

  const auto last_less = prev(first_not_less);
  if (first_not_less == numbers.end()) {
    return last_less;
  }

  // Разыменуем оба итератора-кандидата и выберем тот,
  // чей элемент ближе к искомому
  const bool is_left = (border - *last_less <= *first_not_less - border);
  return is_left ? last_less : first_not_less;
}

int main() {
	set<int> numbers = {1, 4, 6};
	//cout << *FindNearestElement(numbers, 5) << endl;
	cout <<
	  *FindNearestElement(numbers, 0) << " " <<
	  *FindNearestElement(numbers, 3) << " " <<
	  *FindNearestElement(numbers, 5) << " " <<
	  *FindNearestElement(numbers, 6) << " " <<
	  *FindNearestElement(numbers, 100) << endl;

	set<int> empty_set;

	cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << endl;
return 0;
	return 0;
}
/*
Вкратце, из ниженаписанного следует, что 
для итераторов std::set сложность реализации алгоритма std::lower-bound линейная вместо ожидаемой логарифмической. 
Сложность же реализации std::set::lower_bound -- логарифмическая.

Как найти (для тех, кому интересна метода поиска причины просадки в производительности программы):

Исходя из написанного вот здесь (см. раздел Complexity): https://en.cppreference.com/w/cpp/algorithm/lower_bound

The number of comparisons performed is logarithmic in the distance between first and last (At most log 2(last - first) + O(1) comparisons). However, for non-RandomAccessIterators, the number of iterator increments is linear.

Вспоминаем что у Set итератор не-RandomAccess и получаем ответ на наш вопрос.

Русскоязычный аналог документации (https://ru.cppreference.com/w/cpp/algorithm/lower_bound)

вносит "сумятицу в народные массы", говоря что сложность "Логарифмическая в расстоянии между first и last". Объясняется это видимо тем, что автоперевод запнулся на русскоязычной формулировке non-RandomAccessIterators и просто выбросил соответствующий кусок пояснения из документации.

Пойдём дальше и заглянем в документацию по std::set::lower_bound https://en.cppreference.com/w/cpp/container/set/lower_bound (в русскоязычный аналог я бы, увы, после увиденного, смотреть не стал...), опять смотрим раздел Complexity:

здесь уже чётко обещано, что сложность этого метода - логарифм от размера контейнера.
*/
