#include "polywarp/cpu_engine.hpp"
#include <cassert>
#include <iostream>
#include <vector>

using namespace PolyWarp;

int main() {
    std::cout << "[RUNNING] PolyWarp Unit Test: ARM64 to x86 Translation & Execution...\n";

    CPUEngine engine;

    // Test:
    // ADD X1, X0, #100
    // SUB X2, X1, #25
    // RET
    std::vector<uint32_t> arm_code = {
        0x91019001, // ADD X1, X0, #100
        0xD1006422, // SUB X2, X1, #25
        0xD65F03C0  // RET
    };

    std::vector<IRInstruction> ir;
    bool decoded = engine.DecodeARM64(arm_code.data(), arm_code.size(), ir);
    assert(decoded);
    assert(!ir.empty());

    std::vector<uint8_t> x86_code;
    bool emitted = engine.EmitX86(ir, x86_code);
    assert(emitted);

    auto func = engine.PrepareExecutableBlock(x86_code);
    assert(func != nullptr);

    uint64_t registers[32] = {0};
    registers[0] = 50; // Initial X0

    func(registers);

    // X1 should be 50 + 100 = 150
    // X2 should be 150 - 25 = 125
    assert(registers[1] == 150);
    assert(registers[2] == 125);

    std::cout << "[PASSED] Test Complete: X1=150, X2=125 verified!\n";
    return 0;
}
