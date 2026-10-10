#include "ResourceManager/RomExtractor.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string_view>

namespace sclpr::resource {

namespace {

std::string ToLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}

std::string HexDump(const std::vector<uint8_t>& data, size_t offset, size_t length) {
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');
    for (size_t i = 0; i < length && offset + i < data.size(); ++i) {
        if (i > 0) {
            oss << ' ';
        }
        oss << std::setw(2) << static_cast<int>(data[offset + i]);
    }
    return oss.str();
}

} // namespace

bool RomExtractor::LoadRom(const std::string& filePath) {
    std::ifstream stream(filePath, std::ios::binary);
    if (!stream.is_open()) {
        return false;
    }

    stream.seekg(0, std::ios::end);
    const std::streamoff size = stream.tellg();
    stream.seekg(0, std::ios::beg);

    if (size <= 0) {
        return false;
    }

    m_romData.resize(static_cast<size_t>(size));
    stream.read(reinterpret_cast<char*>(m_romData.data()), size);
    m_romPath = filePath;
    return true;
}

std::vector<ExtractedChunk> RomExtractor::ExtractChunksBySignature(const std::vector<std::string>& signatures) const {
    std::vector<ExtractedChunk> chunks;
    if (m_romData.empty()) {
        return chunks;
    }

    for (const auto& signature : signatures) {
        const std::string normalized = ToLower(signature);
        if (normalized.empty()) {
            continue;
        }

        const std::string romLower = ToLower(std::string(reinterpret_cast<const char*>(m_romData.data()), std::min<size_t>(m_romData.size(), 4096)));
        const std::size_t found = romLower.find(normalized);
        if (found == std::string::npos) {
            continue;
        }

        ExtractedChunk chunk;
        chunk.name = "signature_" + normalized;
        chunk.type = "metadata";
        chunk.offset = found;
        chunk.size = std::min<size_t>(256, m_romData.size() - found);
        chunk.data.assign(m_romData.begin() + static_cast<std::ptrdiff_t>(found),
                          m_romData.begin() + static_cast<std::ptrdiff_t>(found + chunk.size));
        chunks.push_back(chunk);
    }

    return chunks;
}

bool RomExtractor::ExtractPaletteTables(const std::filesystem::path& outputDirectory) const {
    if (m_romData.empty()) {
        return false;
    }

    std::filesystem::create_directories(outputDirectory);

    const std::vector<std::string> paletteSignatures = {
        "palette",
        "pal",
        "title",
        "font",
        "art"
    };

    const auto chunks = ExtractChunksBySignature(paletteSignatures);
    if (chunks.empty()) {
        // Fallback: dump the first 32 KB as a palette archive when no signature is found.
        std::filesystem::path fallback = outputDirectory / "palette_fallback.bin";
        std::ofstream out(fallback, std::ios::binary);
        if (!out.is_open()) {
            return false;
        }
        out.write(reinterpret_cast<const char*>(m_romData.data()), std::streamsize(std::min<size_t>(32 * 1024, m_romData.size())));
        out.close();
        return true;
    }

    for (size_t i = 0; i < chunks.size(); ++i) {
        std::filesystem::path filePath = outputDirectory / (chunks[i].name + ".bin");
        std::ofstream out(filePath, std::ios::binary);
        if (!out.is_open()) {
            return false;
        }
        out.write(reinterpret_cast<const char*>(chunks[i].data.data()), static_cast<std::streamsize>(chunks[i].data.size()));
        out.close();
    }

    return true;
}

bool RomExtractor::ExtractRawAssets(const std::filesystem::path& outputDirectory) const {
    if (m_romData.empty()) {
        return false;
    }

    std::filesystem::create_directories(outputDirectory);

    const std::filesystem::path rawPath = outputDirectory / "rom_dump.bin";
    std::ofstream out(rawPath, std::ios::binary);
    if (!out.is_open()) {
        return false;
    }

    out.write(reinterpret_cast<const char*>(m_romData.data()), static_cast<std::streamsize>(m_romData.size()));
    out.close();
    return true;
}

bool RomExtractor::ExportPaletteArchive(const std::filesystem::path& outputDirectory) const {
    if (m_romData.empty()) {
        return false;
    }

    std::filesystem::create_directories(outputDirectory);

    const auto chunks = ExtractChunksBySignature({"palette", "pal", "title"});
    const std::filesystem::path archivePath = outputDirectory / "palette_archive.txt";
    std::ofstream out(archivePath);
    if (!out.is_open()) {
        return false;
    }

    out << "Sonic CD Little Planet Revisited - Palette Archive\n";
    out << "rom_path=" << m_romPath << '\n';
    out << "chunks=" << chunks.size() << '\n';

    for (size_t i = 0; i < chunks.size(); ++i) {
        out << "[" << i << "] name=" << chunks[i].name << " offset=" << chunks[i].offset
            << " size=" << chunks[i].size << " hex=" << HexDump(chunks[i].data, 0, std::min<size_t>(64, chunks[i].data.size())) << '\n';
    }

    out.close();
    return true;
}

const std::vector<uint8_t>& RomExtractor::GetRomData() const noexcept {
    return m_romData;
}

bool RomExtractor::HasRomLoaded() const noexcept {
    return !m_romData.empty();
}

} // namespace sclpr::resource
