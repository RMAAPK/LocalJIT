# PolyWarp: Decentralized Cross-Architecture Execution Engine

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Target: x86__64](https://img.shields.io/badge/Target-Intel%20x86__64%20Bare--Metal-red.svg)]()
[![Graphics: Mesa 4.5](https://img.shields.io/badge/Graphics-OpenGL%204.5%20%2F%20DirectX-green.svg)]()
[![P2P: IPFS Swarm](https://img.shields.io/badge/Vault-IPFS%20Content--Addressed-teal.svg)]()

**PolyWarp** is a high-performance cross-architecture binary and graphics execution engine. It combines bare-metal CPU translation (e.g. ARM64 to native x86_64) and DirectX-to-OpenGL shader interception with a decentralized, content-addressed IPFS cache vault.

---

## ⚡ Core Pillars

1. **Zero Runtime CPU Emulation:**
   - Decodes foreign instructions (ARM64/RISC-V) directly into micro-op Intermediate Representation (IR).
   - Generates and writes raw x86_64 machine code directly into executable pages (`mmap` with `PROT_EXEC`), achieving native execution speed.

2. **Distributed Shader Vault (P2P Mesh):**
   - Intercepts DirectX 9/11 HLSL shader bytecode.
   - Hashes shaders to deterministic Content Identifiers (CIDs).
   - Pre-compiled GPU shaders are shared across the peer swarm over IPFS, eliminating shader compilation stutter during gaming.

3. **Deterministic Content-Addressing:**
   - Every compiled block is immutable, tamper-resistant, and instantly verifiable by hash before memory injection.

---

## 📁 Repository Structure

```text
PolyWarp/
├── CMakeLists.txt                  # C++20 build pipeline
├── README.md                       # Engine documentation & blueprint
├── LICENSE                         # MIT License
├── include/polywarp/               # Public C++20 headers
│   ├── common.hpp                  # Common types & data structures
│   ├── cpu_engine.hpp              # ARM64/RISC-V to x86 translation interface
│   ├── gpu_engine.hpp              # DirectX to OpenGL shader cache
│   └── ipfs_vault.hpp              # Content-addressed P2P storage
├── src/
│   ├── cpu/                        # Instruction decoder, IR builder & JIT page
│   ├── gpu/                        # Shader interception & OpenGL transpilation
│   ├── ipfs/                       # IPFS CID generator & memory vault
│   └── main.cpp                    # Standalone engine runner
└── tests/
    └── test_cpu_translation.cpp    # CPU translation verification suite
```

---

## 🛠️ Building & Running

### Requirements
- Linux (Ubuntu/Debian/Mint) or POSIX-compliant OS
- GCC 13+ or Clang 16+ supporting C++20
- Standard POSIX pthreads and `sys/mman.h`

### Compile & Run Tests
```bash
# Compile and run the verification test suite
g++ -std=c++20 -O3 -Iinclude src/cpu/*.cpp src/gpu/*.cpp src/ipfs/*.cpp tests/test_cpu_translation.cpp -o test_cpu -pthread
./test_cpu
```

### Run the PolyWarp Demonstration Daemon
```bash
g++ -std=c++20 -O3 -Iinclude src/cpu/*.cpp src/gpu/*.cpp src/ipfs/*.cpp src/main.cpp -o polywarp_bin -pthread
./polywarp_bin
```

---

## 📜 License
PolyWarp is open source software released under the [MIT License](LICENSE).
