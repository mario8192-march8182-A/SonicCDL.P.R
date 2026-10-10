#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace sclpr::resource {

class PaletteManager {
public:
    struct Color {
        uint8_t r{0};
        uint8_t g{0};
        uint8_t b{0};
        uint8_t a{255};
    };

    PaletteManager() = default;
    ~PaletteManager() = default;

    PaletteManager(const PaletteManager&) = delete;
    PaletteManager& operator=(const PaletteManager&) = delete;

    // Populate palette data from a raw ROM section.
    bool LoadFromRomData(const std::vector<uint8_t>& romData, size_t offset, size_t length);

    // Create a palette by reading a standardized 16-color or 256-color table.
    bool LoadFromTable(const std::vector<uint8_t>& tableData, size_t entries = 256);

    // Add a palette manually.
    void AddPalette(const std::string& name, const std::vector<Color>& colors);

    // Retrieve palette by name.
    const std::vector<Color>* GetPalette(const std::string& name) const;

    // Export a palette as a basic plaintext palette description.
    bool ExportPalette(const std::string& name, const std::string& filePath) const;

    // Convert a palette to RGBA32 values.
    std::vector<uint32_t> ToRgba32(const std::vector<Color>& colors) const;

    // Clear all loaded palettes.
    void Clear();

private:
    std::vector<std::pair<std::string, std::vector<Color>>> m_palettes;
};

} // namespace sclpr::resource
