#include "ResourceManager/RomAssetCatalog.h"

#include <algorithm>
#include <string_view>

namespace sclpr::resource {

namespace {

std::string ToLower(std::string input) {
    std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return input;
}

AssetKind ClassifyByNameInternal(const std::string& assetName) {
    const std::string lowered = ToLower(assetName);

    if (lowered.find("pal") != std::string::npos || lowered.find("palette") != std::string::npos) {
        return AssetKind::Palette;
    }
    if (lowered.find("sprite") != std::string::npos || lowered.find("obj") != std::string::npos) {
        return AssetKind::Sprite;
    }
    if (lowered.find("tile") != std::string::npos || lowered.find("map") != std::string::npos) {
        return AssetKind::Tile;
    }
    if (lowered.find("music") != std::string::npos || lowered.find("song") != std::string::npos) {
        return AssetKind::Music;
    }
    if (lowered.find("sound") != std::string::npos || lowered.find("sfx") != std::string::npos) {
        return AssetKind::Sound;
    }
    if (lowered.find("level") != std::string::npos || lowered.find("stage") != std::string::npos) {
        return AssetKind::Level;
    }
    if (lowered.find("text") != std::string::npos || lowered.find("string") != std::string::npos) {
        return AssetKind::Text;
    }

    return AssetKind::Binary;
}

} // namespace

std::vector<AssetChunk> RomAssetCatalog::ScanForKnownChunks(const std::vector<uint8_t>& romData,
    const std::vector<std::string>& signatures,
    const std::string& platform) {
    std::vector<AssetChunk> chunks;
    if (romData.empty()) {
        return chunks;
    }

    for (const std::string& signature : signatures) {
        const std::string target = ToLower(signature);
        if (target.empty()) {
            continue;
        }

        const std::string romText(reinterpret_cast<const char*>(romData.data()), std::min<size_t>(romData.size(), 4096));
        const std::string romLower = ToLower(romText);
        const auto offset = romLower.find(target);
        if (offset == std::string::npos) {
            continue;
        }

        AssetChunk chunk;
        chunk.name = signature;
        chunk.type = "known_signature";
        chunk.platform = platform;
        chunk.kind = ClassifyByNameInternal(signature);
        chunk.offset = offset;
        chunk.size = std::min<size_t>(256, romData.size() - offset);
        chunk.bytes.assign(romData.begin() + static_cast<std::ptrdiff_t>(offset),
            romData.begin() + static_cast<std::ptrdiff_t>(offset + chunk.size));
        chunks.push_back(chunk);
    }

    return chunks;
}

std::vector<std::pair<std::string, size_t>> RomAssetCatalog::BuildAssetIndex(const std::vector<uint8_t>& romData,
    const std::vector<std::string>& signatures,
    const std::string& platform) {
    std::vector<std::pair<std::string, size_t>> index;
    const auto chunks = ScanForKnownChunks(romData, signatures, platform);
    index.reserve(chunks.size());

    for (const auto& chunk : chunks) {
        index.emplace_back(chunk.name, chunk.offset);
    }

    return index;
}

std::vector<AssetChunk> RomAssetCatalog::BuildCatalogForBuild(const std::vector<uint8_t>& romData,
    const std::string& platform) {
    std::vector<std::string> signatures = {
        "palette", "pal", "sprite", "tiles", "music", "stage", "level", "text", "sound", "art"
    };

    return ScanForKnownChunks(romData, signatures, platform);
}

AssetKind RomAssetCatalog::ClassifyByName(const std::string& assetName) {
    return ClassifyByNameInternal(assetName);
}

} // namespace sclpr::resource
