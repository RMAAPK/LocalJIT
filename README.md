# PolyWarp: A P2P JIT

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Target: x86__64](https://img.shields.io/badge/Target-Intel%20x86__64%20Bare--Metal-red.svg)]()
[![Graphics: Mesa 4.5](https://img.shields.io/badge/Graphics-OpenGL%204.5%20%2F%20DirectX-green.svg)]()
[![P2P: Swarm Cache](https://img.shields.io/badge/Swarm-P2P%20Content--Addressed%20JIT-teal.svg)]()

**PolyWarp** is a decentralized, peer-to-peer compilation cache and JIT engine (`P2P JIT`). It crowdsources pre-compiled CPU translation blocks and GPU shader pipelines across a decentralized swarm, eliminating runtime compilation stutter for emulators, gaming on Linux, and cross-architecture workloads.

---

## ⚡ The Core Problem: Compilation Stutter
Every time an emulator (PCSX2, RPCS3, Cemu) or translation layer (Wine/DXVK) encounters new machine code or DirectX shader bytecode, it must pause execution to compile it for your local GPU/CPU. 

Valve built a closed-source, centralized shader cache exclusively for Steam. **PolyWarp** turns this into an open, serverless **P2P JIT**:
- **Compile Once, Share with the Swarm:** When Node A compiles a shader or basic block on their AMD or Intel silicon, PolyWarp computes its cryptographic Content Identifier (CID) and announces it to the peer-to-peer swarm.
- **Zero-Stutter Playback:** When Node B encounters the same code block on matching hardware, it pre-fetches the verified native binary ahead of time and hot-patches it directly into executable memory with **zero compilation overhead**.

---

## ⚡ Core Pillars

1. **Zero Runtime CPU Emulation:**
   - Translates foreign instructions (ARM64/RISC-V) directly into micro-op Intermediate Representation (IR).
   - Generates and writes raw x86_64 machine code directly into executable pages (`mmap` with `PROT_EXEC`), achieving native execution speed.

2. **P2P JIT & Distributed Shader Swarm:**
   - Intercepts DirectX 9/11 HLSL shader bytecode.
   - Hashes shaders to deterministic Content Identifiers (CIDs).
   - Pre-compiled GPU shaders are shared across the peer swarm over P2P/IPFS, eliminating shader compilation stutter during gameplay.

3. **Multi-Tier Fallback Hierarchy:**
   - **Tier 1 (L1 Cache):** Instant 1.2–15 ns local memory lookup.
   - **Tier 2 (Airgapped BLE/Radio):** Micro-shard synchronization via TrackOn BLE (0x4C5D) and LoRa.
   - **Tier 3 (P2P Swarm):** Background BitTorrent/IPFS DHT discovery for full binary blocks.

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

