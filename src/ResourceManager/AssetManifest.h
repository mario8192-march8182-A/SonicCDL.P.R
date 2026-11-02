#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

namespace sclpr::resource {

enum class AssetType {
    Palette,
    Sprite,
    Tile,
    Music,
    Sound,
    Stage,
    Text,
    Unknown
};

struct AssetManifestEntry {
    std::string id;
    std::string fileName;
    std::string build;
    std::string path;
    AssetType type{AssetType::Unknown};
    size_t offset{0};
    size_t size{0};
    bool extracted{false};
};

class AssetManifest {
public:
    AssetManifest() = default;
    ~AssetManifest() = default;

    void AddEntry(const AssetManifestEntry& entry);
    void AddEntry(std::string id,
        std::string fileName,
        std::string build,
        std::string path,
        AssetType type,
        size_t offset,
        size_t size,
        bool extracted = false);

    const std::vector<AssetManifestEntry>& GetEntries() const noexcept;
    std::vector<AssetManifestEntry> GetByBuild(const std::string& build) const;
    std::vector<AssetManifestEntry> GetByType(AssetType type) const;

    bool SaveToFile(const std::string& filePath) const;
    bool LoadFromFile(const std::string& filePath);

private:
    std::vector<AssetManifestEntry> m_entries;
};

} // namespace sclpr::resource
