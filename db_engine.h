#ifndef DB_ENGINE_H
#define DB_ENGINE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include <fstream>
#include <filesystem>

namespace db {

    struct Record {
        std::string key;
        std::string value;
        bool deleted;
        uint64_t timestamp;

        Record() : deleted(false), timestamp(0) {}
        Record(std::string k, std::string v)
            : key(std::move(k)), value(std::move(v)), deleted(false), timestamp(0) {
        }
    };

    class MemTable {
    public:
        MemTable(size_t max_size = 1024 * 1024);
        ~MemTable();

        bool put(const std::string& key, const std::string& value);
        bool get(const std::string& key, std::string& value);
        bool del(const std::string& key);
        size_t size() const;
        bool empty() const;
        std::vector<Record> flush();
        void clear();

        size_t get_max_size() const { return max_size_; }

    private:
        std::unordered_map<std::string, Record> table_;
        size_t current_size_;
        size_t max_size_;
        mutable std::mutex mutex_;
    };

    class WAL {
    public:
        explicit WAL(const std::string& path);
        ~WAL();

        bool append(const Record& record);
        bool recover(std::unordered_map<std::string, Record>& table);
        void clear();

    private:
        std::string wal_path_;
        std::fstream wal_file_;
        std::mutex mutex_;

        bool open_file();
        void close_file();
    };

    class SSTable {
    public:
        explicit SSTable(const std::string& dir);
        ~SSTable();

        bool write(const std::vector<Record>& records);
        bool read(const std::string& key, std::string& value);
        std::vector<std::string> list_files();
        std::string get_path() const;

    private:
        std::string directory_;

        std::string generate_filename();
    };

    class CompactionManager {
    public:
        explicit CompactionManager(SSTable* sstable);
        ~CompactionManager();

        void compact();
        bool needs_compaction() const;

    private:
        SSTable* sstable_;
        size_t file_count_threshold_ = 4;

        void merge_files(const std::vector<std::string>& files);
    };

    class DBEngine {
    public:
        DBEngine(const std::string& data_dir, size_t memtable_size = 1024 * 1024);
        ~DBEngine();

        bool put(const std::string& key, const std::string& value);
        bool get(const std::string& key, std::string& value);
        bool del(const std::string& key);
        void flush();
        void compact();

    private:
        std::string data_dir_;
        size_t memtable_size_;
        std::unique_ptr<MemTable> memtable_;
        std::unique_ptr<WAL> wal_;
        std::unique_ptr<SSTable> sstable_;
        std::unique_ptr<CompactionManager> compaction_;
    };

} // namespace db

#endif // DB_ENGINE_H