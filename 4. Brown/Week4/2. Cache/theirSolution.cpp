  }

  Entries::iterator AddEntry(const string& book_name, Entry entry) {
    assert(0 == books_.count(book_name));
    cache_size_ += entry.book->GetContent().size();
    return books_.insert({book_name, move(entry)}).first;
  }

private:
  std::shared_ptr<IBooksUnpacker> books_unpacker_;
  const Settings settings_;

  mutex mutex_;
  Entries books_;
  int current_rank_ = 0;
  size_t cache_size_ = 0;
};


unique_ptr<ICache> MakeCache(
    std::shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
  return make_unique<LruCache>(move(books_unpacker), settings);
}