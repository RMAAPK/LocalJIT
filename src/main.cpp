#include "polywarp/cpu_engine.hpp"
#include "polywarp/gpu_engine.hpp"
#include "polywarp/ipfs_vault.hpp"
#include <iostream>
#include <vector>

using namespace PolyWarp;

int main(int argc, char** argv) {
    std::cout << "========================================================\n";
    std::cout << "  PolyWarp: Decentralized Cross-Architecture Engine v1.0\n";
    std::cout << "  Target: Intel Core i5-3470 (x86_64) | Mesa AMD CAICOS\n";
    std::cout << "========================================================\n\n";

    CPUEngine cpu;
    GPUEngine gpu;
    IPFSVault vault;

    // Test Demonstration: Translate ARM64 sequence:
    // ADD X0, X0, #42
    // RET
    std::vector<uint32_t> arm_code = {
        0x9100A800, // ADD X0, X0, #42 (imm12=42, Rn=0, Rd=0)
        0xD65F03C0  // RET
    };

    std::cout << "[+] Decoding ARM64 block (2 instructions)...\n";
    std::vector<IRInstruction> ir;
    cpu.DecodeARM64(arm_code.data(), arm_code.size(), ir);
    std::cout << "[+] Generated " << ir.size() << " PolyWarp IR micro-ops.\n";

    std::vector<uint8_t> x86_code;
    cpu.EmitX86(ir, x86_code);
    std::cout << "[+] Emitted " << x86_code.size() << " bytes of native x86_64 machine code.\n";

    // Publish compiled artifact to IPFS vault
    std::string cid = vault.GenerateCID(x86_code.data(), x86_code.size());
    vault.PublishArtifact(cid, x86_code);
    std::cout << "[+] Stored compiled artifact in IPFS Vault with CID: " << cid << "\n";

    // Execute directly in JIT page
    auto func = cpu.PrepareExecutableBlock(x86_code);
    if (func) {
        uint64_t registers[32] = {0};
        registers[0] = 58; // Input register X0 = 58
        std::cout << "[+] Calling native JIT block with initial X0 = " << registers[0] << "...\n";
        func(registers);
        std::cout << "[+] Execution Complete! Final Result in X0 = " << registers[0] << " (Expected: 100)\n";
    }

    std::cout << "\n[+] PolyWarp daemon initialized and ready for execution.\n";
    return 0;
}
