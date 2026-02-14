## ✨ Features

CompleteCustomDatabase is packed with features designed for performance, reliability, and ease of use. Below is a comprehensive breakdown of its capabilities.

### 🔧 Core Engine Features
- **🗃️ LSM-Tree Architecture**: Implements a production-grade Log-Structured Merge-Tree, providing superior write performance by converting random writes into sequential disk writes.
- **📝 Write-Ahead Logging (WAL)**: Ensures durability with a pre-write logging mechanism. Every write operation is first recorded in the WAL, guaranteeing zero data loss during crashes or power failures.
- **⚡ Multi-Level Compaction**: Automatic background compaction process that continuously merges SSTables, controls read amplification, and reclaims disk space from deleted/overwritten data.
- **🔍 Efficient Point Lookups**: Optimized read path that checks MemTable → Immutable MemTable → SSTable levels (with binary search within each level) for fast key retrieval.
- **🧠 SkipList MemTable**: In-memory storage using a thread-safe, lock-free SkipList implementation, offering O(log n) complexity for both reads and writes.
- **📊 Bloom Filters (Optional)**: Built-in support for Bloom filters to quickly determine if a key definitely does not exist in an SSTable, significantly reducing unnecessary disk I/O for non-existent keys.
- **🗜️ Configurable Block Compression**: Support for optional compression algorithms (e.g., Snappy) for SSTable blocks, reducing disk space usage at the cost of minimal CPU overhead.

### 🔐 Concurrency & Safety
- **🧵 Fully Thread-Safe**: All public API operations (`Put`, `Get`, `Delete`) are protected by fine-grained mutexes, allowing safe concurrent access from multiple threads without data corruption.
- **🔄 Iterator Support**: Provides a consistent, forward-only iterator interface to traverse key-value pairs across the entire database or within a specific key range.
- **💾 Snapshot Isolation**: Create read-consistent snapshots of the database at a point in time, allowing long-running read operations without blocking writes.

### ⚙️ Configuration & Flexibility
- **📏 Tunable Performance**: Adjust critical parameters to match your workload:
  - `memtable_size`: Control memory usage and flush frequency.
  - `compaction_threshold`: Configure when background compaction triggers.
  - `block_size`: Optimize for point lookups vs. range scans.
  - `wal_enabled`: Toggle durability for maximum write speed (trade-off: crash safety).
- **🗂️ Custom Key Comparator**: Support for custom key comparison functions, enabling advanced use cases like composite keys or custom sorting orders.

### 🛠️ Operational Excellence
- **🔄 Crash Recovery**: On startup, automatically replays the WAL to rebuild the MemTable, ensuring data integrity after unexpected shutdowns.
- **📈 Performance Metrics**: Built-in statistics counters (e.g., number of compactions, cache hits/misses, write amplification) for monitoring and tuning.
- **🧹 Manual Compaction Trigger**: Force immediate compaction of all SSTables or a specific key range for testing or maintenance purposes.
- **💼 Batch Operations**: Atomic batch writes (`WriteBatch`) for multiple `Put`/`Delete` operations, ensuring all or nothing semantics.

### 📦 Developer Experience
- **🎯 Zero External Dependencies**: Pure C++17 implementation using only the Standard Template Library (STL) – no Boost, no third-party libraries.
- **📚 Clean, Documented Codebase**: Well-commented source code with clear separation of concerns, perfect for learning or production use.
- **✅ Comprehensive Testing**: Extensive unit tests covering edge cases (compaction corner cases, recovery scenarios, concurrent access patterns).
- **🔧 Easy Integration**: Simple header includes and a CMake build system – just link and start using.

### 🌐 Platform Compatibility
- **🪟 Windows**: Full support with Visual Studio 2022 and MinGW.
- **🐧 Linux**: Tested on Ubuntu 20.04+, Debian, CentOS with GCC and Clang.
- **🍎 macOS**: Compatible with macOS 10.15+ (Catalina) and newer, Apple Silicon support.

---

### 📊 Performance Characteristics

| Operation | Average Complexity | Description |
|-----------|-------------------|-------------|
| **Put**   | O(log n) | Write to MemTable + append to WAL |
| **Get (existing key)** | O(log n) average | Search MemTable → Immutable → SSTables |
| **Get (non-existent)** | O(1) with Bloom filter | Quick rejection without disk I/O |
| **Delete** | O(log n) | Write a tombstone entry (lazy deletion) |
| **Range Scan** | O(n + log m) | Efficient forward iteration across levels |

---

### 🎯 Use Cases

CompleteCustomDatabase excels in scenarios where:

- **Write-heavy workloads**: Applications with high insert/update rates (e.g., time-series data, log aggregation).
- **Embedded systems**: Need a lightweight database without external process overhead.
- **Prototyping & Learning**: Understanding how real-world storage engines work under the hood.
- **IoT applications**: Resource-constrained environments requiring persistent key-value storage.
- **Caching layers**: Persistent cache with LRU-like behavior (via manual deletion or compaction).

> ⚡ **Performance Tip**: For maximum write throughput, disable the WAL (`wal_enabled = false`) – but remember, you lose crash recovery guarantees!
