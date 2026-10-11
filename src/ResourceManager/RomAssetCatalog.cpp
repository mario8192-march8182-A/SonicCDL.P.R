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

} // namespace

std::vector<AssetChunk> RomAssetCatalog::ScanForKnownChunks(const std::vector<uint8_t>& romData,
    const std::vector<std::string>& signatures) {
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
        chunk.offset = offset;
        chunk.size = std::min<size_t>(256, romData.size() - offset);
        chunk.bytes.assign(romData.begin() + static_cast<std::ptrdiff_t>(offset),
                           romData.begin() + static_cast<std::ptrdiff_t>(offset + chunk.size));
        chunks.push_back(chunk);
    }

    return chunks;
}

std::vector<std::pair<std::string, size_t>> RomAssetCatalog::BuildAssetIndex(const std::vector<uint8_t>& romData,
    const std::vector<std::string>& signatures) {
    std::vector<std::pair<std::string, size_t>> index;
    const auto chunks = ScanForKnownChunks(romData, signatures);
    index.reserve(chunks.size());

    for (const auto& chunk : chunks) {
        index.emplace_back(chunk.name, chunk.offset);
    }

    return index;
}

} // namespace sclpr::resource
