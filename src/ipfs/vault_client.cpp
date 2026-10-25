#include "polywarp/ipfs_vault.hpp"
#include <sstream>
#include <iomanip>

namespace PolyWarp {

IPFSVault::IPFSVault(const std::string& local_cache_dir)
    : m_cache_dir(local_cache_dir) {}

IPFSVault::~IPFSVault() {}

std::string IPFSVault::GenerateCID(const uint8_t* data, size_t size) {
    // Generate deterministic Content ID (CIDv1-style base58 simulation)
    uint64_t hash1 = 14695981039346656037ULL;
    uint64_t hash2 = 1099511628211ULL;
    for (size_t i = 0; i < size; ++i) {
        hash1 = (hash1 ^ data[i]) * 16777619;
        hash2 = (hash2 + data[i]) * 31;
    }

    std::stringstream ss;
    ss << "bafybeic" << std::hex << std::setw(16) << std::setfill('0') << hash1
       << std::hex << std::setw(16) << std::setfill('0') << hash2;
    return ss.str();
}

bool IPFSVault::PublishArtifact(const std::string& cid, const std::vector<uint8_t>& data) {
    m_memory_cache[cid] = data;
    return true;
}

std::optional<std::vector<uint8_t>> IPFSVault::FetchArtifact(const std::string& cid) {
    auto it = m_memory_cache.find(cid);
    if (it != m_memory_cache.end()) {
        return it->second;
    }
    return std::nullopt;
}

} // namespace PolyWarp
