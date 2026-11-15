#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace sclpr::resource {

enum class AddOnRomKind {
    Unknown,
    Sonic3AndKnuckles,
    Sonic2,
    Sonic3DBlast
};

struct AddOnRomInfo {
    AddOnRomKind kind{AddOnRomKind::Unknown};
    std::filesystem::path path;
    std::string detectedTitle;
    bool headerMatched{false};

    // These describe intended integrations. They do not mean ROM assets or
    // stages have already been decoded/imported by this registry.
    bool providesPlayableCharacter{false};
    bool providesSpinDashReference{false};
    bool providesStageReference{false};
    bool requires2DStageConversion{false};
};

// Registers user-supplied Mega Drive/Genesis ROM paths as optional reference
// sources. This class does not extract, copy, or distribute ROM contents.
class AddOnRomManager {
public:
    bool RegisterRom(const std::filesystem::path& romPath, AddOnRomInfo& outInfo);
    bool RemoveRom(const std::filesystem::path& romPath);

    const std::vector<AddOnRomInfo>& GetRegisteredRoms() const noexcept;
    const AddOnRomInfo* Find(AddOnRomKind kind) const noexcept;

private:
    static AddOnRomInfo InspectRom(const std::filesystem::path& romPath);

    std::vector<AddOnRomInfo> m_roms;
};

} // namespace sclpr::resource
