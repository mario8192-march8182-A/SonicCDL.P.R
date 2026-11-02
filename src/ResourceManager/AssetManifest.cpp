#include "ResourceManager/AssetManifest.h"

#include <fstream>
#include <sstream>

namespace sclpr::resource {

void AssetManifest::AddEntry(const AssetManifestEntry& entry) {
    m_entries.push_back(entry);
}

void AssetManifest::AddEntry(std::string id,
    std::string fileName,
    std::string build,
    std::string path,
    AssetType type,
    size_t offset,
    size_t size,
    bool extracted) {
    AssetManifestEntry entry;
    entry.id = std::move(id);
    entry.fileName = std::move(fileName);
    entry.build = std::move(build);
    entry.path = std::move(path);
    entry.type = type;
    entry.offset = offset;
    entry.size = size;
    entry.extracted = extracted;
    m_entries.push_back(entry);
}

const std::vector<AssetManifestEntry>& AssetManifest::GetEntries() const noexcept {
    return m_entries;
}

std::vector<AssetManifestEntry> AssetManifest::GetByBuild(const std::string& build) const {
    std::vector<AssetManifestEntry> out;
    for (const auto& entry : m_entries) {
        if (entry.build == build) {
            out.push_back(entry);
        }
    }
    return out;
}

std::vector<AssetManifestEntry> AssetManifest::GetByType(AssetType type) const {
    std::vector<AssetManifestEntry> out;
    for (const auto& entry : m_entries) {
        if (entry.type == type) {
            out.push_back(entry);
        }
    }
    return out;
}

bool AssetManifest::SaveToFile(const std::string& filePath) const {
    std::ofstream out(filePath);
    if (!out.is_open()) {
        return false;
    }

    for (const auto& entry : m_entries) {
        out << entry.id << ","
            << entry.fileName << ","
            << entry.build << ","
            << entry.path << ","
            << static_cast<int>(entry.type) << ","
            << entry.offset << ","
            << entry.size << ","
            << (entry.extracted ? 1 : 0) << '\n';
    }

    return true;
}

bool AssetManifest::LoadFromFile(const std::string& filePath) {
    std::ifstream in(filePath);
    if (!in.is_open()) {
        return false;
    }

    m_entries.clear();
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        std::string id, fileName, build, path, typeStr, offsetStr, sizeStr, extractedStr;
        std::getline(ss, id, ',');
        std::getline(ss, fileName, ',');
        std::getline(ss, build, ',');
        std::getline(ss, path, ',');
        std::getline(ss, typeStr, ',');
        std::getline(ss, offsetStr, ',');
        std::getline(ss, sizeStr, ',');
        std::getline(ss, extractedStr, ',');

        AssetManifestEntry entry;
        entry.id = id;
        entry.fileName = fileName;
        entry.build = build;
        entry.path = path;
        entry.type = static_cast<AssetType>(std::stoi(typeStr));
        entry.offset = static_cast<size_t>(std::stoull(offsetStr));
        entry.size = static_cast<size_t>(std::stoull(sizeStr));
        entry.extracted = std::stoi(extractedStr) != 0;
        m_entries.push_back(entry);
    }

    return true;
}

} // namespace sclpr::resource
