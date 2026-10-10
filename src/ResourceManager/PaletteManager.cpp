#include "ResourceManager/PaletteManager.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace sclpr::resource {

bool PaletteManager::LoadFromRomData(const std::vector<uint8_t>& romData, size_t offset, size_t length) {
    if (offset >= romData.size()) {
        return false;
    }

    const size_t available = std::min(length, romData.size() - offset);
    std::vector<Color> colors;
    colors.reserve(available / 2);

    for (size_t i = 0; i + 1 < available; i += 2) {
        const uint8_t low = romData[offset + i];
        const uint8_t high = romData[offset + i + 1];

        // Simple conversion for 5-5-5 RGB style from ROM-like byte pairs.
        const uint8_t r = static_cast<uint8_t>((low & 0x1F) << 3);
        const uint8_t g = static_cast<uint8_t>(((high & 0x03) << 5) | ((low >> 5) & 0x07) << 3);
        const uint8_t b = static_cast<uint8_t>((high >> 2) << 3);
        colors.push_back(Color{r, g, b, 255});
    }

    AddPalette("rom_palette", colors);
    return true;
}

bool PaletteManager::LoadFromTable(const std::vector<uint8_t>& tableData, size_t entries) {
    if (tableData.empty()) {
        return false;
    }

    std::vector<Color> colors;
    colors.reserve(entries);

    const size_t usableEntries = std::min(entries, tableData.size() / 3);
    for (size_t i = 0; i < usableEntries; ++i) {
        const size_t base = i * 3;
        colors.push_back(Color{
            tableData[base],
            tableData[base + 1],
            tableData[base + 2],
            255
        });
    }

    AddPalette("table_palette", colors);
    return true;
}

void PaletteManager::AddPalette(const std::string& name, const std::vector<Color>& colors) {
    std::vector<std::pair<std::string, std::vector<Color>>>::iterator it = std::find_if(
        m_palettes.begin(),
        m_palettes.end(),
        [&name](const auto& entry) {
            return entry.first == name;
        });

    if (it != m_palettes.end()) {
        it->second = colors;
        return;
    }

    m_palettes.emplace_back(name, colors);
}

const std::vector<PaletteManager::Color>* PaletteManager::GetPalette(const std::string& name) const {
    const auto it = std::find_if(m_palettes.begin(), m_palettes.end(), [&name](const auto& entry) {
        return entry.first == name;
    });

    if (it == m_palettes.end()) {
        return nullptr;
    }

    return &it->second;
}

bool PaletteManager::ExportPalette(const std::string& name, const std::string& filePath) const {
    const auto palette = GetPalette(name);
    if (palette == nullptr) {
        return false;
    }

    std::ofstream out(filePath, std::ios::binary);
    if (!out.is_open()) {
        return false;
    }

    out << "palette_name=" << name << '\n';
    out << "colors=" << palette->size() << '\n';
    for (size_t i = 0; i < palette->size(); ++i) {
        out << i << ": "
            << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
            << static_cast<int>((*palette)[i].r) << std::setw(2)
            << static_cast<int>((*palette)[i].g) << std::setw(2)
            << static_cast<int>((*palette)[i].b) << '\n';
    }

    out.close();
    return true;
}

std::vector<uint32_t> PaletteManager::ToRgba32(const std::vector<Color>& colors) const {
    std::vector<uint32_t> rgba;
    rgba.reserve(colors.size());

    for (const auto& color : colors) {
        const uint32_t packed =
            (static_cast<uint32_t>(color.a) << 24) |
            (static_cast<uint32_t>(color.r) << 16) |
            (static_cast<uint32_t>(color.g) << 8) |
            static_cast<uint32_t>(color.b);
        rgba.push_back(packed);
    }

    return rgba;
}

void PaletteManager::Clear() {
    m_palettes.clear();
}

} // namespace sclpr::resource
