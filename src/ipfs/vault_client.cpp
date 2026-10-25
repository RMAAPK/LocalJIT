#include "polywarp/ipfs_vault.hpp"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace PolyWarp {

IPFSVault::IPFSVault(const std::string& local_cache_dir)
    : m_cache_dir(local_cache_dir) {
    fs::create_directories(m_cache_dir);
}

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
    // 1. Commit to in-memory L1 cache
    m_memory_cache[cid] = data;

    // 2. Commit to local self-hosted vault storage
    std::string filepath = m_cache_dir + "/" + cid + ".bin";
    std::ofstream f(filepath, std::ios::binary);
    if (f) {
        f.write(reinterpret_cast<const char*>(data.data()), data.size());
    }
    return true;
}

std::optional<std::vector<uint8_t>> IPFSVault::FetchArtifact(const std::string& cid) {
    // 1. Check in-memory cache
    auto it = m_memory_cache.find(cid);
    if (it != m_memory_cache.end()) {
        return it->second;
    }

    // 2. Check local disk vault
    std::string filepath = m_cache_dir + "/" + cid + ".bin";
    if (fs::exists(filepath)) {
        std::ifstream f(filepath, std::ios::binary);
        if (f) {
            std::vector<uint8_t> buf((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
            m_memory_cache[cid] = buf;
            return buf;
        }
    }

    return std::nullopt;
}

} // namespace PolyWarp

