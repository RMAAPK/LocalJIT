#pragma once

#include <string>
#include <vector>
#include <optional>
#include <cstdint>

namespace PolyWarp {

enum class NetworkTier {
    LOCAL_CACHE,    // L1 / SQLite RAM cache
    BLE_MESH,       // TrackOn airgapped 2.4 GHz radio transport (0x4C5D)
    LORA_LONG_RANGE,// Sub-GHz LoRa (433/868/915 MHz) for 10-15km long-range hops
    FM_RDS_BURST,   // VHF FM Broadcast subcarrier (64-108 MHz RDS/RBDS) 1187.5 bps
    INTERNET_IPFS   // Public/Private IPFS Gateway / Web P2P Swarm
};

struct PolyWarpPacket {
    std::string cid;
    std::vector<uint8_t> payload;
    NetworkTier tier_source;
    bool is_verified{false};
};

class MultiTierMeshRouter {
public:
    MultiTierMeshRouter(
        const std::string& trackon_chat_db = "/media/thealidev/RMAA HDD1/RMAAPK/TrackOn Chat/trackon_chat.db",
        const std::string& trackon_tracker_dir = "/media/thealidev/RMAA HDD1/RMAAPK/TrackOn Tacker/trackon"
    );
    ~MultiTierMeshRouter();

    // Query across tiers: Local -> BLE Mesh -> LoRa -> FM RDS -> Internet IPFS
    std::optional<std::vector<uint8_t>> ResolveArtifact(const std::string& cid);

    // Ingest and broadcast an artifact according to size constraints across tiers
    bool DistributeArtifact(const std::string& cid, const std::vector<uint8_t>& data);

    // Physical Radio Layer Transports
    bool SyncWithTrackOnBLE();
    bool TransmitLoRaPacket(const std::vector<uint8_t>& packet);
    bool BroadcastFMRDS(const std::vector<uint8_t>& packet);

private:
    std::string m_trackon_chat_db;
    std::string m_trackon_tracker_dir;
};

} // namespace PolyWarp
