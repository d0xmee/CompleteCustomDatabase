#include "db_engine.h"
#include <map>
#include <iostream>

namespace db {

    CompactionManager::CompactionManager(SSTable* sstable) : sstable_(sstable) {}

    CompactionManager::~CompactionManager() = default;

    bool CompactionManager::needs_compaction() const {
        auto files = sstable_->list_files();
        return files.size() >= file_count_threshold_;
    }

    void CompactionManager::compact() {
        if (!needs_compaction()) return;

        auto files = sstable_->list_files();
        std::sort(files.begin(), files.end());

        std::vector<std::string> files_to_merge;
        for (size_t i = 0; i < files.size() - 1; ++i) {
            files_to_merge.push_back(files[i]);
        }

        merge_files(files_to_merge);
    }

    void CompactionManager::merge_files(const std::vector<std::string>& files) {
        std::map<std::string, Record> merged_records;

        for (const auto& file : files) {
            std::ifstream infile(file, std::ios::binary);
            if (!infile.is_open()) continue;

            uint64_t count;
            infile.read(reinterpret_cast<char*>(&count), sizeof(count));

            for (uint64_t i = 0; i < count; ++i) {
                uint32_t key_len, val_len;
                uint8_t del;
                uint64_t timestamp;

                infile.read(reinterpret_cast<char*>(&key_len), sizeof(key_len));
                std::string key(key_len, '\0');
                infile.read(&key[0], key_len);

                infile.read(reinterpret_cast<char*>(&val_len), sizeof(val_len));
                std::string val(val_len, '\0');
                infile.read(&val[0], val_len);

                infile.read(reinterpret_cast<char*>(&del), sizeof(del));
                infile.read(reinterpret_cast<char*>(&timestamp), sizeof(timestamp));

                auto it = merged_records.find(key);
                if (it == merged_records.end() || it->second.timestamp < timestamp) {
                    Record rec;
                    rec.key = key;
                    rec.value = val;
                    rec.deleted = (del != 0);
                    rec.timestamp = timestamp;
                    merged_records[key] = std::move(rec);
                }
            }
        }

        std::vector<Record> records;
        records.reserve(merged_records.size());

        for (const auto& pair : merged_records) {
            if (!pair.second.deleted) {
                records.push_back(pair.second);
            }
        }

        std::string new_file = sstable_->get_path() + "/compacted_" +
            std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) +
            ".dat";

        std::ofstream outfile(new_file, std::ios::binary);
        if (outfile.is_open()) {
            uint64_t count = records.size();
            outfile.write(reinterpret_cast<const char*>(&count), sizeof(count));

            for (const auto& rec : records) {
                uint32_t key_len = static_cast<uint32_t>(rec.key.size());
                uint32_t val_len = static_cast<uint32_t>(rec.value.size());
                uint8_t deleted = 0;

                outfile.write(reinterpret_cast<const char*>(&key_len), sizeof(key_len));
                outfile.write(rec.key.data(), key_len);
                outfile.write(reinterpret_cast<const char*>(&val_len), sizeof(val_len));
                outfile.write(rec.value.data(), val_len);
                outfile.write(reinterpret_cast<const char*>(&deleted), sizeof(deleted));
                outfile.write(reinterpret_cast<const char*>(&rec.timestamp), sizeof(rec.timestamp));
            }
        }

        for (const auto& file : files) {
            std::filesystem::remove(file);
        }
    }

} // namespace db