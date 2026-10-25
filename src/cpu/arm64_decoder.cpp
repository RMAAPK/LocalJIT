#include "polywarp/cpu_engine.hpp"

namespace PolyWarp {

bool CPUEngine::DecodeARM64(const uint32_t* arm_insts, size_t count, std::vector<IRInstruction>& out_ir) {
    for (size_t i = 0; i < count; ++i) {
        uint32_t inst = arm_insts[i];

        // ADD (64-bit immediate): 1001 0001 xx imm12 Rn Rd
        if ((inst & 0xFF800000) == 0x91000000) {
            uint8_t rd = inst & 0x1F;
            uint8_t rn = (inst >> 5) & 0x1F;
            uint16_t imm12 = (inst >> 10) & 0xFFF;
            uint8_t shift = (inst >> 22) & 0x3;
            int64_t val = (shift == 1) ? (imm12 << 12) : imm12;

            out_ir.push_back({IROp::LOAD_REG, 0, rn, 0, 0, 0});
            out_ir.push_back({IROp::ADD, 0, 0, 0, val, 0});
            out_ir.push_back({IROp::STORE_REG, rd, 0, 0, 0, 0});
            continue;
        }

        // SUB (64-bit immediate): 1101 0001 xx imm12 Rn Rd
        if ((inst & 0xFF800000) == 0xD1000000) {
            uint8_t rd = inst & 0x1F;
            uint8_t rn = (inst >> 5) & 0x1F;
            uint16_t imm12 = (inst >> 10) & 0xFFF;
            uint8_t shift = (inst >> 22) & 0x3;
            int64_t val = (shift == 1) ? (imm12 << 12) : imm12;

            out_ir.push_back({IROp::LOAD_REG, 0, rn, 0, 0, 0});
            out_ir.push_back({IROp::SUB, 0, 0, 0, val, 0});
            out_ir.push_back({IROp::STORE_REG, rd, 0, 0, 0, 0});
            continue;
        }

        // RET: 1101 0110 0101 1111 0000 00 Rn 00000 (0xD65F0000)
        if ((inst & 0xFFFFFC1F) == 0xD65F0000) {
            out_ir.push_back({IROp::RET, 0, 0, 0, 0, 0});
            continue;
        }
    }
    return !out_ir.empty();
}

} // namespace PolyWarp
