#include "db_engine.h"
#include <chrono>

namespace db {

    MemTable::MemTable(size_t max_size) : current_size_(0), max_size_(max_size) {}

    MemTable::~MemTable() = default;

    bool MemTable::put(const std::string& key, const std::string& value) {
        std::lock_guard<std::mutex> lock(mutex_);

        uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();

        size_t new_size = key.size() + value.size();

        auto it = table_.find(key);
        if (it != table_.end()) {
            current_size_ -= (it->second.key.size() + it->second.value.size());
        }

        Record rec;
        rec.key = key;
        rec.value = value;
        rec.timestamp = timestamp;
        rec.deleted = false;

        table_[key] = std::move(rec);
        current_size_ += new_size;

        return true;
    }

    bool MemTable::get(const std::string& key, std::string& value) {
        std::lock_guard<std::mutex> lock(mutex_);

        auto it = table_.find(key);
        if (it != table_.end() && !it->second.deleted) {
            value = it->second.value;
            return true;
        }

        return false;
    }

    bool MemTable::del(const std::string& key) {
        std::lock_guard<std::mutex> lock(mutex_);

        uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();

        auto it = table_.find(key);
        if (it != table_.end()) {
            it->second.deleted = true;
            it->second.timestamp = timestamp;
        }
        else {
            Record rec;
            rec.key = key;
            rec.deleted = true;
            rec.timestamp = timestamp;
            table_[key] = std::move(rec);
            current_size_ += key.size();
        }

        return true;
    }

    size_t MemTable::size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return current_size_;
    }

    bool MemTable::empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return table_.empty();
    }

    std::vector<Record> MemTable::flush() {
        std::lock_guard<std::mutex> lock(mutex_);

        std::vector<Record> records;
        records.reserve(table_.size());

        for (const auto& pair : table_) {
            records.push_back(pair.second);
        }

        table_.clear();
        current_size_ = 0;

        return records;
    }

    void MemTable::clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        table_.clear();
        current_size_ = 0;
    }

} // namespace db