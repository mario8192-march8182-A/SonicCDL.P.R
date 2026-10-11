#pragma once

#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <unordered_map>
#include <nlohmann/json.hpp>

namespace sclpr::mod {

using json = nlohmann::json;

struct ModMetadata {
    std::string name;
    std::string author;
    std::string version;
    std::string description;
    std::vector<std::string> tags;
    bool enabled{true};
};

struct ModManifest {
    ModMetadata metadata;
    std::filesystem::path modPath;
    std::unordered_map<std::string, std::string> assets;
    std::vector<std::string> scripts;
};

class ModLoader {
public:
    ModLoader() = default;
    ~ModLoader() = default;

    ModLoader(const ModLoader&) = delete;
    ModLoader& operator=(const ModLoader&) = delete;

    // Scan for all mods in the Mods/ directory.
    std::vector<ModManifest> ScanMods(const std::filesystem::path& modsDirectory);

    // Load a specific mod by name.
    bool LoadMod(const std::string& modName);

    // Load all enabled mods.
    bool LoadAllMods();

    // Get loaded mod manifests.
    const std::vector<ModManifest>& GetLoadedMods() const noexcept;

    // Get a specific mod by name.
    const ModManifest* FindMod(const std::string& modName) const;

    // Unload all mods.
    void UnloadAll();

private:
    bool ParseModJson(const std::filesystem::path& modPath, ModManifest& outManifest);

    std::vector<ModManifest> m_loadedMods;
    std::unordered_map<std::string, size_t> m_modIndex;
};

} // namespace sclpr::mod
