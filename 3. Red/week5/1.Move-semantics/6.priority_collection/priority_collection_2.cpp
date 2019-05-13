#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#include <map>
#include <deque>

using namespace std;

template <typename T>
class PriorityCollection {
public:
    using Id = typename deque<T>::iterator;

    PriorityCollection() : max_pr(0) {}

    // Добавить объект с нулевым приоритетом
    // с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        objects.push_back(move(object));
        Id tmp = --objects.end();
        priorById[tmp] = 0;
        idByPrior[0].insert(tmp);
        if( max_pr == 0 ) {
            max_id = tmp;
        }
        return tmp;
    }

    // Добавить все элементы диапазона [range_begin, range_end)
    // с помощью перемещения, записав выданные им идентификаторы
    // в диапазон [ids_begin, ...)
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
            IdOutputIt ids_begin) {
        int before_size = objects.size();
        copy(make_move_iterator(range_begin), make_move_iterator(range_end),
             back_inserter(objects));
        for( auto it = objects.begin()+before_size; it != objects.end(); ++it) {
            priorById[it] = 0;
            idByPrior[0].insert(it);
            if( max_pr == 0 ) {
                max_id = it;
            }
            *ids_begin = it;
            ids_begin++;
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо
    // хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        return priorById.count(id);
    }

    // Получить объект по идентификатору
    const T& Get(Id id) const {
        return (*id);
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        int old_pr = priorById[id];
        idByPrior[old_pr].erase(id);
        idByPrior[++old_pr].insert(id);
        priorById[id]++;
        if ( old_pr > max_pr ||
             ( old_pr == max_pr && id > max_id )) {
            max_pr = old_pr;
            max_id = id;
        }
    }

    // Получить объект с максимальным приоритетом и его приоритет
    pair<const T&, int> GetMax() const {
        return { *max_id, max_pr };
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax() {
        pair<T, int> res(move(*max_id), max_pr);
        auto max_it = --idByPrior[max_pr].end();
        idByPrior[max_pr].erase(max_it);
        priorById.erase(max_id);
        while ( idByPrior[max_pr].size() == 0 && max_pr > 0 ) {
            max_pr -= 1;
        }
        if( idByPrior[max_pr].size() > 0 ) {
            max_id = *(--(idByPrior[max_pr].end()));
        }

        return res;
    }

private:
    deque<T> objects;
    map<Id, int> priorById;
    map<int, set<Id>> idByPrior;
    int max_pr;
    Id max_id;
};

class StringNonCopyable : public string {
public:
    using string::string; // Позволяет использовать конструкторы строки
    StringNonCopyable(const StringNonCopyable&) = delete;
    StringNonCopyable(StringNonCopyable&&) = default;
    StringNonCopyable& operator=(const StringNonCopyable&) = delete;
    StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
    PriorityCollection<StringNonCopyable> strings;
    const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");

    strings.Promote(yellow_id);
    for (int i = 0; i < 2; ++i) {
        strings.Promote(red_id);
    }
    strings.Promote(yellow_id);
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white");
        ASSERT_EQUAL(item.second, 0);
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    return 0;
}
