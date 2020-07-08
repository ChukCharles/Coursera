#include "test_runner.h"

#include <numeric>
#include <iostream>
#include <vector>
#include <string>
#include <future>
using namespace std;

template <typename Iterator>
struct IteratorRange {
	Iterator first, last;

	IteratorRange(Iterator f, Iterator l)
		: first(f), last(l) {}

	Iterator begin() const {
		return first;
	}
	Iterator end() const {
		return last;
	}
	size_t size() const { return last - first; }
};

template <typename Iterator>
class Paginator {
public:
	Paginator(Iterator f, Iterator l, size_t s)
		: page_size(s), first(f), last(l)
	{
		Iterator tmp = f;
		while (tmp != l) {
			Iterator second = tmp;
			if ((l - tmp) < s) {
				second = l;
			}
			else {
				second = next(tmp, s);
			}
			pages.push_back(IteratorRange(tmp, second));
			tmp = second;
		}
	}
	auto begin() const {
		return pages.begin();
	}
	auto end() const {
		return pages.end();
	}
	size_t size() const { return pages.size(); }
private:
	int page_size;
	Iterator first, last;
	vector<IteratorRange<Iterator>> pages;
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
	return Paginator{ c.begin(), c.end(), page_size };
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
    // Реализуйте эту функцию
	size_t psize = matrix.size()/4+1;
	vector<future<int64_t>> futures;
	int64_t result = 0;
	for (auto subcontainer : Paginate(matrix, psize)) {
		//cout << *subcontainer.begin() << " " << *prev(subcontainer.end()) << endl;
		futures.push_back(async([subcontainer] {
			int64_t res = 0;
			for (auto vect : subcontainer) {
				res += accumulate(vect.begin(), vect.end(), 0);
			}
			return res;
		}));
	}
	for (future<int64_t>& f : futures) result += f.get();
	return result;
}

void TestCalculateMatrixSum() {
    const vector<vector<int>> matrix = {
      {1, 2, 3, 4},
      {5, 6, 7, 8},
      {9, 10, 11, 12},
      {13, 14, 15, 16}
    };
    ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestCalculateMatrixSum);
}
