#include "polywarp/cpu_engine.hpp"

namespace PolyWarp {

bool CPUEngine::EmitX86(const std::vector<IRInstruction>& ir, std::vector<uint8_t>& out_x86) {
    // Standard System V AMD64 ABI:
    // First argument (regs pointer) arrives in %rdi
    // Return value in %rax

    for (const auto& inst : ir) {
        switch (inst.op) {
            case IROp::LOAD_REG: {
                // mov (%rdi, rn, 8), %rax
                out_x86.push_back(0x48); // REX.W
                out_x86.push_back(0x8B); // MOV r64, r/m64
                out_x86.push_back(0x47); // ModR/M [rdi + disp8]
                out_x86.push_back(inst.src_reg * 8);
                break;
            }
            case IROp::ADD: {
                // add $imm, %rax
                out_x86.push_back(0x48); // REX.W
                out_x86.push_back(0x05); // ADD RAX, imm32
                uint32_t imm32 = static_cast<uint32_t>(inst.immediate);
                out_x86.push_back(imm32 & 0xFF);
                out_x86.push_back((imm32 >> 8) & 0xFF);
                out_x86.push_back((imm32 >> 16) & 0xFF);
                out_x86.push_back((imm32 >> 24) & 0xFF);
                break;
            }
            case IROp::SUB: {
                // sub $imm, %rax
                out_x86.push_back(0x48); // REX.W
                out_x86.push_back(0x2D); // SUB RAX, imm32
                uint32_t imm32 = static_cast<uint32_t>(inst.immediate);
                out_x86.push_back(imm32 & 0xFF);
                out_x86.push_back((imm32 >> 8) & 0xFF);
                out_x86.push_back((imm32 >> 16) & 0xFF);
                out_x86.push_back((imm32 >> 24) & 0xFF);
                break;
            }
            case IROp::STORE_REG: {
                // mov %rax, (%rdi, rd, 8)
                out_x86.push_back(0x48); // REX.W
                out_x86.push_back(0x89); // MOV r/m64, r64
                out_x86.push_back(0x47); // ModR/M [rdi + disp8]
                out_x86.push_back(inst.dst_reg * 8);
                break;
            }
            case IROp::RET: {
                // ret
                out_x86.push_back(0xC3);
                break;
            }
            default:
                break;
        }
    }
    return !out_x86.empty();
}

} // namespace PolyWarp
