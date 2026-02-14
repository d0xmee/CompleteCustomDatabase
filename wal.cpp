#include "db_engine.h"
#include <iostream>

namespace db {

    WAL::WAL(const std::string& path) : wal_path_(path) {
        open_file();
    }

    WAL::~WAL() {
        close_file();
    }

    bool WAL::open_file() {
        wal_file_.open(wal_path_, std::ios::binary | std::ios::app | std::ios::in | std::ios::out);
        if (!wal_file_.is_open()) {
            wal_file_.open(wal_path_, std::ios::binary | std::ios::trunc | std::ios::out);
            wal_file_.close();
            wal_file_.open(wal_path_, std::ios::binary | std::ios::app | std::ios::in | std::ios::out);
        }
        return wal_file_.is_open();
    }

    void WAL::close_file() {
        if (wal_file_.is_open()) {
            wal_file_.close();
        }
    }

    bool WAL::append(const Record& record) {
        std::lock_guard<std::mutex> lock(mutex_);

        if (!wal_file_.is_open() && !open_file()) {
            return false;
        }

        uint32_t key_len = static_cast<uint32_t>(record.key.size());
        uint32_t val_len = static_cast<uint32_t>(record.value.size());
        uint8_t deleted = record.deleted ? 1 : 0;

        wal_file_.write(reinterpret_cast<const char*>(&key_len), sizeof(key_len));
        wal_file_.write(record.key.data(), key_len);
        wal_file_.write(reinterpret_cast<const char*>(&val_len), sizeof(val_len));
        wal_file_.write(record.value.data(), val_len);
        wal_file_.write(reinterpret_cast<const char*>(&deleted), sizeof(deleted));
        wal_file_.write(reinterpret_cast<const char*>(&record.timestamp), sizeof(record.timestamp));
        wal_file_.flush();

        return wal_file_.good();
    }

    bool WAL::recover(std::unordered_map<std::string, Record>& table) {
        std::lock_guard<std::mutex> lock(mutex_);

        close_file();
        wal_file_.open(wal_path_, std::ios::binary | std::ios::in);

        if (!wal_file_.is_open()) {
            return false;
        }

        table.clear();

        while (!wal_file_.eof()) {
            uint32_t key_len, val_len;
            uint8_t deleted;
            uint64_t timestamp;

            wal_file_.read(reinterpret_cast<char*>(&key_len), sizeof(key_len));
            if (wal_file_.gcount() < sizeof(key_len)) break;

            std::string key(key_len, '\0');
            wal_file_.read(&key[0], key_len);
            if (wal_file_.gcount() < key_len) break;

            wal_file_.read(reinterpret_cast<char*>(&val_len), sizeof(val_len));
            if (wal_file_.gcount() < sizeof(val_len)) break;

            std::string val(val_len, '\0');
            wal_file_.read(&val[0], val_len);
            if (wal_file_.gcount() < val_len) break;

            wal_file_.read(reinterpret_cast<char*>(&deleted), sizeof(deleted));
            if (wal_file_.gcount() < sizeof(deleted)) break;

            wal_file_.read(reinterpret_cast<char*>(&timestamp), sizeof(timestamp));
            if (wal_file_.gcount() < sizeof(timestamp)) break;

            Record rec;
            rec.key = key;
            rec.value = val;
            rec.deleted = (deleted != 0);
            rec.timestamp = timestamp;

            auto it = table.find(key);
            if (it == table.end() || it->second.timestamp < timestamp) {
                table[key] = std::move(rec);
            }
        }

        wal_file_.close();
        open_file();

        return true;
    }

    void WAL::clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        close_file();
        std::filesystem::remove(wal_path_);
        open_file();
    }

} // namespace db