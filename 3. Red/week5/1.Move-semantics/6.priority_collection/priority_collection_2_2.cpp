#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <string>

using namespace std;

template <typename T>
class PriorityCollection {
public:
    using Id = int;

    // Добавить объект с нулевым приоритетом
    // с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        _data.push_back(move(object));
        _priorities[0].insert(_data.size() - 1);
        _ids.push_back(0);
        Id id = _data.size() - 1;
        return id;
    }

    // Добавить все элементы диапазона [range_begin, range_end)
    // с помощью перемещения, записав выданные им идентификаторы
    // в диапазон [ids_begin, ...)
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
        IdOutputIt ids_begin) {
        int first_id = _data.size();
        int last_id = first_id + (range_end - range_begin);
        _data.reserve(range_end - range_begin);
        move(range_begin, range_end, back_inserter(_data));
        for (first_id; first_id <= last_id; first_id++) {
            _ids.push_back(0);
            _priorities[0].insert(first_id);
            *ids_begin = first_id;
            ids_begin++;
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо
    // хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        if (id < _ids.size() && _ids[id] >= 0) return true;
        return false;
    }

    // Получить объект по идентификатору
    const T& Get(Id id) const { return _data[id]; }

    // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        int old_pr = _ids[id];
        _ids[id]++;
        _priorities[old_pr].erase(id);
        _priorities[old_pr+1].insert(id);
        if (old_pr + 1 > _max_priority) _max_priority++;
    }

    // Получить объект с максимальным приоритетом и его приоритет
    pair<const T&, int> GetMax() const {
        Id max_id = *(_priorities[_max_priority].end()--);
        return make_pair(_data[max_id], _max_priority);
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax() {
        Id max_id = *(prev(_priorities[_max_priority].end()));
        int max_pr = _max_priority;
        _priorities[_max_priority].erase(max_id);
        while (_priorities[_max_priority].size() == 0
            && _max_priority > 0) {
            _max_priority--;
        }
        _ids[max_id] = -1;
        return make_pair(move(_data[max_id]), max_pr);
    }

private:
    vector<T> _data;
    vector<int> _ids;
    map<int, set<Id>> _priorities;
    int _max_priority = 0;
};


class StringNonCopyable : public string {
public:
    using string::string;  // Позволяет использовать конструкторы строки
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
    //PriorityCollection<StringNonCopyable> strings;
    /*const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");

    strings.Promote(yellow_id);
    for (int i = 0; i < 2; ++i) {
        strings.Promote(red_id);
    }
    strings.Promote(yellow_id);
    const auto item = strings.PopMax();*/

    return 0;
}
