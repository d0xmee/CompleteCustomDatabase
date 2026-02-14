<div align="center">
  <h1>⚡ CompleteCustomDatabase</h1>
  <p><strong>A lightweight, embeddable key-value storage engine with LSM-tree architecture</strong></p>
  
  <!-- Badges -->
  <p>
    <img src="https://img.shields.io/badge/C%2B%2B-17-blue?style=for-the-badge&logo=c%2B%2B" alt="C++17">
    <img src="https://img.shields.io/badge/license-MIT-green?style=for-the-badge&logo=opensourceinitiative" alt="MIT License">
    <img src="https://img.shields.io/badge/version-1.0.0-orange?style=for-the-badge&logo=semver" alt="Version 1.0.0">
    <img src="https://img.shields.io/badge/build-passing-brightgreen?style=for-the-badge&logo=githubactions" alt="Build Passing">
  </p>
  
  <p>
    <img src="https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white" alt="Windows">
    <img src="https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black" alt="Linux">
    <img src="https://img.shields.io/badge/macOS-000000?style=for-the-badge&logo=apple&logoColor=white" alt="macOS">
  </p>
  
  <p>
    <a href="https://github.com/d0xmee/CompleteCustomDatabase">
      <img src="https://img.shields.io/badge/GitHub-d0xmee-181717?style=for-the-badge&logo=github" alt="GitHub">
    </a>
  </p>
</div>

---

## 📋 Table of Contents
- [About The Project](#-about-the-project)
- [Tech Stack](#-tech-stack)
- [Features](#-features)

---

## 🎯 About The Project

**CompleteCustomDatabase** is a high-performance, embeddable key-value store inspired by Google's LevelDB and Facebook's RocksDB. It implements an LSM-tree (Log-Structured Merge-Tree) architecture that optimizes write operations while maintaining efficient reads.

### Why this project?
- 💡 **Learn by doing**: Understand how modern databases work internally
- ⚡ **Performance**: Optimized for write-heavy workloads
- 🔧 **Embeddable**: Link directly to your C++ applications
- 🎓 **Educational**: Clean codebase perfect for learning systems programming

---

## 🛠️ Tech Stack

| Category | Technologies |
|----------|-------------|
| **Languages** | ![C++](https://img.shields.io/badge/C++-17-00599C?style=flat-square&logo=c%2B%2B) |
| **Frameworks & Libraries** | ![STL](https://img.shields.io/badge/STL-C++%20Standard%20Library-004482?style=flat-square) ![CMake](https://img.shields.io/badge/CMake-3.15-064F8C?style=flat-square&logo=cmake) |
| **Platforms** | ![Windows](https://img.shields.io/badge/Windows-10%2F11-0078D6?style=flat-square&logo=windows) ![Linux](https://img.shields.io/badge/Linux-Ubuntu%2FDebian-FCC624?style=flat-square&logo=linux) ![macOS](https://img.shields.io/badge/macOS-10.15%2B-000000?style=flat-square&logo=apple) |
| **Tools** | ![Visual Studio](https://img.shields.io/badge/Visual%20Studio-2022-5C2D91?style=flat-square&logo=visualstudio) ![CLion](https://img.shields.io/badge/CLion-2023.3-000000?style=flat-square&logo=clion) ![VSCode](https://img.shields.io/badge/VSCode-1.85-007ACC?style=flat-square&logo=visualstudiocode) ![Git](https://img.shields.io/badge/Git-F05032?style=flat-square&logo=git) |
| **CI/CD** | ![GitHub Actions](https://img.shields.io/badge/GitHub%20Actions-2088FF?style=flat-square&logo=githubactions) |

---

## ✨ Features

### Core Features
- ✅ **Key-Value Storage**: Simple put/get/delete operations
- ✅ **LSM-Tree Architecture**: MemTable + SSTables for efficient writes
- ✅ **WAL (Write-Ahead Log)**: Durability guarantee, crash recovery
- ✅ **Compaction**: Automatic background merging of SSTables
- ✅ **Thread-Safe**: Mutex-protected operations
- ✅ **Memory-Efficient**: Configurable MemTable size
- ✅ **Persistent Storage**: Data survives program restarts
- ✅ **No External Dependencies**: Pure C++17 with STL

### Key Benefits

| Benefit | Description |
|---------|-------------|
| ⚡ **Lightning Fast** | Optimized write performance with LSM-tree |
| 🛡️ **Crash Recovery** | WAL guarantees no data loss |
| 🔄 **Auto Compaction** | Self-maintaining storage |
| 🔒 **Thread Safe** | Mutex-protected operations |
| ⚙️ **Configurable** | Adjust MemTable size, thresholds |
| 📦 **Embeddable** | No external dependencies |
