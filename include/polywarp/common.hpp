#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <memory>

namespace PolyWarp {

enum class Architecture {
    ARM64,
    X86_64,
    RISCV64
};

enum class GraphicsAPI {
    DIRECTX_9,
    DIRECTX_11,
    OPENGL_45,
    VULKAN
};

struct ExecutionBlock {
    uint64_t virtual_address;
    std::vector<uint8_t> native_code;
    std::string ipfs_cid;
    size_t hit_count{0};
};

} // namespace PolyWarp
