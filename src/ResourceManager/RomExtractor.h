#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <filesystem>

namespace sclpr::resource {

struct ExtractedChunk {
    std::string name;
    std::string type;
    size_t offset{0};
    size_t size{0};
    std::vector<uint8_t> data;
};

class RomExtractor {
public:
    RomExtractor() = default;
    ~RomExtractor() = default;

    RomExtractor(const RomExtractor&) = delete;
    RomExtractor& operator=(const RomExtractor&) = delete;

    // Load a ROM for extraction.
    bool LoadRom(const std::string& filePath);

    // Extract basic data blocks that are usually represented as a header or tables.
    std::vector<ExtractedChunk> ExtractChunksBySignature(const std::vector<std::string>& signatures) const;

    // Extract palette-like data blocks to disk.
    bool ExtractPaletteTables(const std::filesystem::path& outputDirectory) const;

    // Extract a general raw asset dump to a folder.
    bool ExtractRawAssets(const std::filesystem::path& outputDirectory) const;

    // Export a decoded palette into a simple text-based palette file.
    bool ExportPaletteArchive(const std::filesystem::path& outputDirectory) const;

    const std::vector<uint8_t>& GetRomData() const noexcept;
    bool HasRomLoaded() const noexcept;

private:
    std::vector<uint8_t> m_romData;
    std::string m_romPath;
};

} // namespace sclpr::resource
