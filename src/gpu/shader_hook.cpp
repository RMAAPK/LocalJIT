#include "polywarp/gpu_engine.hpp"
#include <sstream>
#include <iomanip>

namespace PolyWarp {

GPUEngine::GPUEngine() {}
GPUEngine::~GPUEngine() {}

std::string GPUEngine::ComputeShaderHash(const void* bytecode, size_t length) {
    // 64-bit FNV-1a Hash as fast local key
    const uint8_t* data = static_cast<const uint8_t*>(bytecode);
    uint64_t hash = 14695981039346656037ULL;
    for (size_t i = 0; i < length; ++i) {
        hash ^= data[i];
        hash *= 1099511628211ULL;
    }

    std::stringstream ss;
    ss << "shdr_" << std::hex << std::setw(16) << std::setfill('0') << hash;
    return ss.str();
}

bool GPUEngine::HasCachedShader(const std::string& shader_hash) const {
    return m_shader_cache.find(shader_hash) != m_shader_cache.end();
}

ShaderArtifact GPUEngine::GetCachedShader(const std::string& shader_hash) {
    auto it = m_shader_cache.find(shader_hash);
    if (it != m_shader_cache.end()) {
        return it->second;
    }
    return {};
}

void GPUEngine::CacheShader(const ShaderArtifact& artifact) {
    m_shader_cache[artifact.shader_hash] = artifact;
}

} // namespace PolyWarp
