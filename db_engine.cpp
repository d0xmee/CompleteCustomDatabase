#include "db_engine.h"
#include <chrono>
#include <algorithm>

namespace db {

    DBEngine::DBEngine(const std::string& data_dir, size_t memtable_size)
        : data_dir_(data_dir), memtable_size_(memtable_size) {

        memtable_ = std::make_unique<MemTable>(memtable_size);
        wal_ = std::make_unique<WAL>(data_dir + "/wal.log");
        sstable_ = std::make_unique<SSTable>(data_dir + "/sstables");
        compaction_ = std::make_unique<CompactionManager>(sstable_.get());

        std::unordered_map<std::string, Record> recovered;
        if (wal_->recover(recovered)) {
            for (const auto& pair : recovered) {
                if (!pair.second.deleted) {
                    memtable_->put(pair.second.key, pair.second.value);
                }
            }
        }
    }

    DBEngine::~DBEngine() {
        flush();
    }

    bool DBEngine::put(const std::string& key, const std::string& value) {
        Record rec(key, value);
        rec.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();

        if (!wal_->append(rec)) {
            return false;
        }

        if (!memtable_->put(key, value)) {
            return false;
        }

        if (memtable_->size() >= memtable_size_) {
            flush();
        }

        return true;
    }

    bool DBEngine::get(const std::string& key, std::string& value) {
        if (memtable_->get(key, value)) {
            return true;
        }

        return sstable_->read(key, value);
    }

    bool DBEngine::del(const std::string& key) {
        Record rec;
        rec.key = key;
        rec.deleted = true;
        rec.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();

        if (!wal_->append(rec)) {
            return false;
        }

        return memtable_->del(key);
    }

    void DBEngine::flush() {
        if (memtable_->empty()) return;

        auto records = memtable_->flush();
        sstable_->write(records);
        wal_->clear();

        if (compaction_->needs_compaction()) {
            compact();
        }
    }

    void DBEngine::compact() {
        compaction_->compact();
    }

} // namespace db