#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>

namespace PolyWarp {

class IPFSVault {
public:
    IPFSVault(const std::string& local_cache_dir = "./ipfs_cache");
    ~IPFSVault();

    // Content-addressing hash generation
    std::string GenerateCID(const uint8_t* data, size_t size);

    // Store artifact into local vault and simulate IPFS swarm advertisement
    bool PublishArtifact(const std::string& cid, const std::vector<uint8_t>& data);

    // Retrieve artifact by CID
    std::optional<std::vector<uint8_t>> FetchArtifact(const std::string& cid);

private:
    std::string m_cache_dir;
    std::unordered_map<std::string, std::vector<uint8_t>> m_memory_cache;
};

} // namespace PolyWarp
