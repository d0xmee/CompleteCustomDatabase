# Custom Database Engine

A lightweight, embeddable key-value storage engine written in modern C++17. Implements an LSM-tree (Log-Structured Merge-Tree) architecture with WAL (Write-Ahead Logging) for durability and SSTables for persistent storage.

## 🚀 Features

- **Key-Value Storage**: Simple put/get/delete operations
- **LSM-Tree Architecture**: MemTable + SSTables for efficient writes
- **WAL (Write-Ahead Log)**: Durability guarantee, crash recovery
- **Compaction**: Automatic background merging of SSTables
- **Thread-Safe**: Mutex-protected operations
- **Memory-Efficient**: Configurable MemTable size
- **Persistent Storage**: Data survives program restarts
- **No External Dependencies**: Pure C++17 with STL

## 📋 Requirements

- C++17 compatible compiler (MSVC, GCC, Clang)
- CMake 3.15 or higher (for building)
- Visual Studio 2019/2022 (Windows) or any C++ IDE

## 🛠️ Building the Project

### Using CMake

```bash
# Clone the repository
git clone https://github.com/d0xmee/custom-database-engine.git
cd custom-database-engine

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build .

# Run example
./db_example
