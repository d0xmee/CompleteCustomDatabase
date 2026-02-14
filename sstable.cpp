#include "db_engine.h"
#include <algorithm>
#include <iostream>

namespace db {

    SSTable::SSTable(const std::string& dir) : directory_(dir) {
        std::filesystem::create_directories(dir);
    }

    SSTable::~SSTable() = default;

    std::string SSTable::generate_filename() {
        auto now = std::chrono::system_clock::now();
        auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()
        ).count();

        return directory_ + "/sstable_" + std::to_string(timestamp) + ".dat";
    }

    bool SSTable::write(const std::vector<Record>& records) {
        if (records.empty()) return true;

        std::string filename = generate_filename();
        std::ofstream file(filename, std::ios::binary);

        if (!file.is_open()) return false;

        uint64_t count = records.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(count));

        for (const auto& rec : records) {
            uint32_t key_len = static_cast<uint32_t>(rec.key.size());
            uint32_t val_len = static_cast<uint32_t>(rec.value.size());
            uint8_t deleted = rec.deleted ? 1 : 0;

            file.write(reinterpret_cast<const char*>(&key_len), sizeof(key_len));
            file.write(rec.key.data(), key_len);
            file.write(reinterpret_cast<const char*>(&val_len), sizeof(val_len));
            file.write(rec.value.data(), val_len);
            file.write(reinterpret_cast<const char*>(&deleted), sizeof(deleted));
            file.write(reinterpret_cast<const char*>(&rec.timestamp), sizeof(rec.timestamp));
        }

        return file.good();
    }

    bool SSTable::read(const std::string& key, std::string& value) {
        auto files = list_files();
        std::sort(files.begin(), files.end(), std::greater<std::string>());

        uint64_t latest_timestamp = 0;
        bool found = false;
        std::string found_value;
        bool deleted = false;

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
                std::string curr_key(key_len, '\0');
                infile.read(&curr_key[0], key_len);

                infile.read(reinterpret_cast<char*>(&val_len), sizeof(val_len));
                std::string curr_val(val_len, '\0');
                infile.read(&curr_val[0], val_len);

                infile.read(reinterpret_cast<char*>(&del), sizeof(del));
                infile.read(reinterpret_cast<char*>(&timestamp), sizeof(timestamp));

                if (curr_key == key && timestamp > latest_timestamp) {
                    latest_timestamp = timestamp;
                    found = true;
                    found_value = curr_val;
                    deleted = (del != 0);
                }
            }
        }

        if (found && !deleted) {
            value = found_value;
            return true;
        }

        return false;
    }

    std::vector<std::string> SSTable::list_files() {
        std::vector<std::string> files;

        for (const auto& entry : std::filesystem::directory_iterator(directory_)) {
            if (entry.path().extension() == ".dat") {
                files.push_back(entry.path().string());
            }
        }

        return files;
    }

    std::string SSTable::get_path() const {
        return directory_;
    }

} // namespace db