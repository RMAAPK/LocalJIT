#pragma once

#include "common.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace PolyWarp {

struct ShaderArtifact {
    std::string shader_hash;
    std::string ipfs_cid;
    GraphicsAPI source_api;
    GraphicsAPI target_api;
    std::string compiled_source;
};

class GPUEngine {
public:
    GPUEngine();
    ~GPUEngine();

    // Intercept shader bytecode and compute cryptographic fingerprint
    std::string ComputeShaderHash(const void* bytecode, size_t length);

    // Retrieve from local cache or mark for IPFS fetch
    bool HasCachedShader(const std::string& shader_hash) const;
    ShaderArtifact GetCachedShader(const std::string& shader_hash);

    // Register a newly compiled shader
    void CacheShader(const ShaderArtifact& artifact);

private:
    std::unordered_map<std::string, ShaderArtifact> m_shader_cache;
};

} // namespace PolyWarp

