#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <map>
#include <string>

using namespace std;
template <typename T>
class PriorityCollection {
public:
    using Id = int;

    // Добавить объект с нулевым приоритетом
    // с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        Id idx = data_.size();
        data_.push_back(move(object));
        priority_by_id_[idx] = 0;
        element_by_proirity_[0].insert(idx);
        return idx;
    }

    // Добавить все элементы диапазона [range_begin, range_end)
    // с помощью перемещения, записав выданные им идентификаторы
    // в диапазон [ids_begin, ...)
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
        IdOutputIt ids_begin) {
        Id first_idx = data_.size();
        ObjInputIt it = range_begin;
        while (it != range_end) {
            Id idx = data_.size();
            *(ids_begin++) = idx;
            priority_by_id_[idx] = 0;
            element_by_proirity_[0].insert(idx);

            data_.push_back(move(*it++));
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо
    // хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        if (priority_by_id_.count(id)>0) return true;
        return false;
    }

    // Получить объект по идентификатору
    const T& Get(Id id) const {
        return data_[id];
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        int old_pr = priority_by_id_[id];
        auto it = find(element_by_proirity_[old_pr].begin(),
                       element_by_proirity_[old_pr].end(),
                       id);

        element_by_proirity_[old_pr].erase(it);
        if (element_by_proirity_[old_pr].size() == 0) element_by_proirity_.erase(old_pr);
        int new_pr = ++old_pr;
        element_by_proirity_[new_pr].insert(id);
        priority_by_id_[id] = new_pr;
    }

    // Получить объект с максимальным приоритетом и его приоритет
    pair<const T&, int> GetMax() const {
        auto max = --element_by_proirity_.end();
        int max_priority = max->first;
        int max_element_id = *(--max->second.end());
        return { data_[max_element_id], max_priority };
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax() {
        auto max = --element_by_proirity_.end();
        int max_priority = max->first;
        int max_element_id = *(--max->second.end());

        priority_by_id_.erase(max_element_id);
        element_by_proirity_[max_priority].erase(max_element_id);
        if (element_by_proirity_[max_priority].size() == 0) element_by_proirity_.erase(max_priority);

        return make_pair(move(data_[max_element_id]), max_priority);
    }

private:
    vector<T> data_;
    map<Id, int> priority_by_id_;
    map<int, set<Id>> element_by_proirity_;
};


class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  //StringNonCopyable(const StringNonCopyable&) { 
  //    cout << "copy c-tor" << endl;
  //};
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  //StringNonCopyable& operator=(const StringNonCopyable&) { cout << "operator =" << endl; };
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

void TestConstructionFromRange() {
    PriorityCollection<StringNonCopyable> strings;
    vector<StringNonCopyable> source;
    source.push_back("white");
    source.push_back("yellow");
    source.push_back("red");
    vector<PriorityCollection<StringNonCopyable>::Id> ids(3);


    strings.Add(source.begin(), source.end(), ids.begin());
    //for (auto id : ids) cout << id << " ";
    strings.Promote(1);
    //strings.GetMax();
    //cout << max.second << endl;
}

int main() {
    //TestRunner tr;
    //RUN_TEST(tr, TestNoCopy);
    //RUN_TEST(tr, TestConstructionFromRange);
    auto collection = std::make_unique<PriorityCollection<StringNonCopyable>>();
    std::vector<StringNonCopyable> arr;
    arr.push_back("0");
    arr.push_back("1");
    arr.push_back("2");
    arr.push_back("3");

    using ID = PriorityCollection<StringNonCopyable>::Id;
    std::vector<ID> id_arr;
    collection->Add(arr.begin(), arr.end(), std::back_inserter(id_arr));
    //std::cout << id_arr.size() << '\n';
    //for (auto x : id_arr) {
    //    cout << x << " " << endl;
    //}

    auto maximum = collection->GetMax();
    std::cout << maximum.first << ' ' << maximum.second << '\n';
    return 0;
}
