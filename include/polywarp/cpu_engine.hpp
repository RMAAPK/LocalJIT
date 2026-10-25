#pragma once

#include "common.hpp"
#include <functional>
#include <unordered_map>

namespace PolyWarp {

// Intermediate Representation OpCodes
enum class IROp {
    LOAD_REG,
    STORE_REG,
    ADD,
    SUB,
    MUL,
    JUMP_INDIRECT,
    RET,
    SYSCALL
};

struct IRInstruction {
    IROp op;
    uint8_t dst_reg;
    uint8_t src_reg;
    uint8_t src_reg2;
    int64_t immediate;
    uint64_t target_addr;
};

class CPUEngine {
public:
    CPUEngine();
    ~CPUEngine();

    // Decode foreign instructions to PolyWarp IR
    bool DecodeARM64(const uint32_t* arm_insts, size_t count, std::vector<IRInstruction>& out_ir);

    // Emit native x86_64 machine code from IR
    bool EmitX86(const std::vector<IRInstruction>& ir, std::vector<uint8_t>& out_x86);

    // Allocate executable memory page and execute
    using EntryFunc = uint64_t(*)(uint64_t* regs);
    EntryFunc PrepareExecutableBlock(const std::vector<uint8_t>& x86_bytes);

private:
    void* m_exec_page{nullptr};
    size_t m_page_size{4096};
};

} // namespace PolyWarp

