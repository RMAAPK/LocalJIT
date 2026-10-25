# Local JIT: Self-Hosted & Peer-to-Peer Compilation Cache

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Organization: RMAA Holdings](https://img.shields.io/badge/Organization-RMAA%20Holdings-purple.svg)](https://github.com/RMAAPK)
[![Target: x86__64](https://img.shields.io/badge/Target-Intel%20x86__64%20Bare--Metal-red.svg)]()
[![Graphics: Mesa 4.5](https://img.shields.io/badge/Graphics-OpenGL%204.5%20%2F%20DirectX-green.svg)]()
[![P2P: Swarm Cache](https://img.shields.io/badge/Vault-Self--Hosted%20P2P%20JIT-teal.svg)]()

**Local JIT** (maintained by **RMAA Holdings**) is an open-source, self-hostable, and peer-to-peer compilation cache and dynamic execution engine. It crowdsources pre-compiled CPU translation blocks and GPU shader pipelines across private servers and local networks, eliminating runtime compilation stutter for emulators, gaming on Linux, and cross-architecture workloads.

---

## ⚡ The Core Problem: Compilation Stutter
Every time an emulator (PCSX2, RPCS3, Cemu) or translation layer (Wine/DXVK) encounters new machine code or DirectX shader bytecode, it must pause execution to compile it for your local GPU/CPU. 

Valve built a closed-source, centralized shader cache exclusively for Steam. **Local JIT** turns this into an open, self-hostable, zero-knowledge architecture:
- **Compile Once, Share Locally:** When Node A compiles a shader or basic block on their AMD or Intel silicon, Local JIT computes its cryptographic Content Identifier (CID) and syncs it with the self-hosted vault.
- **Zero-Stutter Playback:** When Node B encounters the same code block on matching hardware, it pre-fetches the verified native binary ahead of time and hot-patches it directly into executable memory with **zero compilation overhead**.

---

## ⚡ Core Pillars

1. **Zero Runtime CPU Emulation:**
   - Translates foreign instructions (ARM64/RISC-V) directly into micro-op Intermediate Representation (IR).
   - Generates and writes raw x86_64 machine code directly into executable pages (`mmap` with `PROT_EXEC`), achieving native execution speed.

2. **Self-Hosted Zero-Knowledge Vault:**
   - Intercepts DirectX 9/11 HLSL shader bytecode and hashes shaders to deterministic Content Identifiers (CIDs).
   - The server stores content-addressed binary blobs with client-side verification, ensuring zero server-operator liability.

3. **Multi-Tier Fallback Hierarchy:**
   - **Tier 1 (L1 Cache):** Instant 1.2–15 ns local memory lookup.
   - **Tier 2 (Airgapped Radio):** Micro-shard synchronization via TrackOn BLE (0x4C5D), LoRa Sub-GHz, and FM RDS.
   - **Tier 3 (Self-Hosted / P2P Vault):** Background REST/P2P vault synchronization for full binary blocks.

---

## 🛡️ Architecture Model: Self-Hosted & Zero-Knowledge (Ente/Bitwarden Model)

To avoid corporate legal entanglements and copyright claims surrounding game shaders and proprietary binary translation:
- **Zero-Knowledge Storage:** Local JIT operates on content-addressed cryptographic hashes (CIDs). The server stores arbitrary encrypted byte arrays and has zero visibility into whether an artifact is a game shader, machine code, or sensor data.
- **Self-Hostable First:** Users, gaming communities, or private LAN clubs run their own Local JIT Vault server (`docker compose up -d`).
- **No Central Liability:** The Local JIT core client connects to whatever private endpoint the user configures (`http://localhost:7470` or private VPS), insulating RMAA Holdings and the project maintainers from third-party hosting liabilities.

### Quickstart: Self-Hosted Vault Server
```bash
cd deploy
docker compose up -d
```
The server will be live on `http://localhost:7470` ready to store and serve pre-compiled artifacts to your local network.

---

## 📁 Repository Structure

```text
Local-JIT/
├── CMakeLists.txt                  # C++20 build pipeline
├── README.md                       # Engine documentation & blueprint
├── LICENSE                         # MIT License (RMAA Holdings)
├── deploy/                         # Self-hosted Docker deployment
│   ├── Dockerfile
│   └── docker-compose.yml
├── server/                         # Zero-knowledge Python/REST vault server
│   └── server.py
├── include/polywarp/               # Public C++20 headers
│   ├── common.hpp                  # Common types & data structures
│   ├── cpu_engine.hpp              # ARM64/RISC-V to x86 translation interface
│   ├── gpu_engine.hpp              # DirectX to OpenGL shader cache
│   ├── mesh_router.hpp             # Multi-tier BLE/LoRa/FM/IPFS router
│   └── ipfs_vault.hpp              # Content-addressed P2P storage
├── src/
│   ├── cpu/                        # Instruction decoder, IR builder & JIT page
│   ├── gpu/                        # Shader interception & OpenGL transpilation
│   ├── ipfs/                       # CID generator & memory vault
│   ├── mesh/                       # Multi-tier radio and mesh handlers
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

### Run the Local JIT Demonstration Daemon
```bash
g++ -std=c++20 -O3 -Iinclude src/cpu/*.cpp src/gpu/*.cpp src/ipfs/*.cpp src/mesh/*.cpp src/main.cpp -o local_jit_bin -pthread
./local_jit_bin
```

---

## 📜 License
Local JIT is open source software released under the [MIT License](LICENSE) by RMAA Holdings & Contributors.
