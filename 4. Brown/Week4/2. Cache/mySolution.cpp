#include "Common.h"
#include<unordered_map>
#include<utility>
#include<string>
#include<mutex>

using namespace std;

class LruCache : public ICache {
public:
    LruCache(
        shared_ptr<IBooksUnpacker> books_unpacker,
        const Settings& settings
    ) : unpacker_(move(books_unpacker)), max_cache_size_(settings.max_memory)
    {
        // реализуйте метод
    }

    BookPtr GetBook(const string& book_name) override {
        lock_guard<mutex> guard(m_);
        if (cache_.find(book_name) != cache_.end()) {
            if (index_[book_name] < max_index_) {
                index_[book_name] = ++max_index_;
            }
            return cache_[book_name];
        }
        else {
            std::unique_ptr<IBook> book = unpacker_->UnpackBook(book_name);
            BookPtr ptr(book.release());
            if (ptr->GetContent().size() > max_cache_size_) {
                cache_.clear();
                index_.clear();
                cache_size_ = 0;
                max_index_ = 0;
                return ptr;
            }
            auto min_book = std::make_pair(std::string(""), max_index_);
            while (max_cache_size_ - cache_size_ < ptr->GetContent().size() && cache_.size() > 0) {
                min_book = std::make_pair(std::string(""), max_index_);
                for (auto& item : index_) {
                    if (item.second <= min_book.second) {
                        min_book = item;
                    }
                }
                //auto r_book = cache_.find(min_book.first);
                cache_size_ -= cache_[min_book.first]->GetContent().size();
                index_.erase(min_book.first);
                cache_.erase(min_book.first);
            }
            //update index
            if (min_book.first.size() > 0) {
                //max index removed
                int new_max = min_book.second;
                for (auto& item : index_) {
                    item.second -= min_book.second;
                    if (item.second >= new_max) new_max = item.second;
                }
                max_index_ = new_max;
            }
            //add(?) book to cache
            if (max_cache_size_ - cache_size_ >= ptr->GetContent().size()) {
                cache_[book_name] = ptr;
                index_[book_name] = ++max_index_;
                cache_size_ += ptr->GetContent().size();
            }
            return ptr;
        }
    }
private:
    size_t max_cache_size_;
    shared_ptr<IBooksUnpacker> unpacker_;

    unordered_map<string, BookPtr> cache_;
    size_t cache_size_ = 0;
    unordered_map<string, int> index_;
    int max_index_ = 0;

    mutex m_;
};


unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
    return std::make_unique<LruCache>(books_unpacker, settings);
}
