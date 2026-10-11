#include "Mod/ModLoader.h"

#include <fstream>
#include <sstream>

namespace sclpr::mod {

std::vector<ModManifest> ModLoader::ScanMods(const std::filesystem::path& modsDirectory) {
    std::vector<ModManifest> discovered;

    if (!std::filesystem::exists(modsDirectory)) {
        return discovered;
    }

    for (const auto& entry : std::filesystem::directory_iterator(modsDirectory)) {
        if (!entry.is_directory()) {
            continue;
        }

        const auto modJsonPath = entry.path() / "mod.json";
        if (!std::filesystem::exists(modJsonPath)) {
            continue;
        }

        ModManifest manifest;
        if (ParseModJson(entry.path(), manifest)) {
            discovered.push_back(manifest);
        }
    }

    return discovered;
}

bool ModLoader::LoadMod(const std::string& modName) {
    const auto it = m_modIndex.find(modName);
    if (it == m_modIndex.end()) {
        return false;
    }

    // Mod is already loaded if it's in the index.
    return true;
}

bool ModLoader::LoadAllMods() {
    // All mods are considered loaded after scanning.
    // Future: add actual asset loading logic here.
    return true;
}

const std::vector<ModManifest>& ModLoader::GetLoadedMods() const noexcept {
    return m_loadedMods;
}

const ModManifest* ModLoader::FindMod(const std::string& modName) const {
    const auto it = m_modIndex.find(modName);
    if (it == m_modIndex.end()) {
        return nullptr;
    }

    return &m_loadedMods[it->second];
}

void ModLoader::UnloadAll() {
    m_loadedMods.clear();
    m_modIndex.clear();
}

bool ModLoader::ParseModJson(const std::filesystem::path& modPath, ModManifest& outManifest) {
    const auto jsonPath = modPath / "mod.json";
    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        return false;
    }

    try {
        json j;
        file >> j;

        outManifest.metadata.name = j.value("name", "");
        outManifest.metadata.author = j.value("author", "");
        outManifest.metadata.version = j.value("version", "1.0.0");
        outManifest.metadata.description = j.value("description", "");
        outManifest.metadata.enabled = j.value("enabled", true);
        outManifest.modPath = modPath;

        if (j.contains("tags") && j["tags"].is_array()) {
            outManifest.metadata.tags = j["tags"].get<std::vector<std::string>>();
        }

        if (j.contains("assets") && j["assets"].is_object()) {
            for (const auto& [key, value] : j["assets"].items()) {
                outManifest.assets[key] = value.get<std::string>();
            }
        }

        if (j.contains("scripts") && j["scripts"].is_array()) {
            outManifest.scripts = j["scripts"].get<std::vector<std::string>>();
        }

        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

} // namespace sclpr::mod
