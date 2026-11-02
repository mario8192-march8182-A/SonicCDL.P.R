#include "ResourceManager/RomBuildAdapter.h"

namespace sclpr::resource {

RomBuildAdapter::RomBuildAdapter(RomBuildVersion buildVersion)
    : m_buildVersion(buildVersion) {}

std::string RomBuildAdapter::GetPlatformName() const noexcept {
    switch (m_buildVersion) {
        case RomBuildVersion::SonicCD_2011:
            return "sega_sonic_cd_2011";
        case RomBuildVersion::SonicCD_Prototype:
            return "sega_sonic_cd_prototype";
        case RomBuildVersion::SonicCD_Classic:
            return "sega_sonic_cd_classic";
        case RomBuildVersion::SonicCD_1993:
            return "sega_sonic_cd_1993";
        default:
            return "generic";
    }
}

std::string RomBuildAdapter::GetLayoutId() const noexcept {
    switch (m_buildVersion) {
        case RomBuildVersion::SonicCD_2011:
            return "layout_2011";
        case RomBuildVersion::SonicCD_Prototype:
            return "layout_prototype";
        case RomBuildVersion::SonicCD_Classic:
            return "layout_classic";
        case RomBuildVersion::SonicCD_1993:
            return "layout_1993";
        default:
            return "layout_unknown";
    }
}

std::vector<std::string> RomBuildAdapter::GetAssetSignatures() const noexcept {
    switch (m_buildVersion) {
        case RomBuildVersion::SonicCD_2011:
            return {"palette", "sprite", "tile", "stage", "music", "sound", "ui", "intro"};
        case RomBuildVersion::SonicCD_Prototype:
            return {"palette", "sprite", "tile", "prototype", "menu", "music", "data"};
        case RomBuildVersion::SonicCD_Classic:
        case RomBuildVersion::SonicCD_1993:
            return {"palette", "pal", "tile", "map", "stage", "title", "music", "sfx"};
        default:
            return {"palette", "pal", "tile", "map", "title", "music"};
    }
}

std::vector<std::string> RomBuildAdapter::GetScanPaths() const noexcept {
    switch (m_buildVersion) {
        case RomBuildVersion::SonicCD_2011:
            return {"assets/2011", "rom/2011", "builds/2011"};
        case RomBuildVersion::SonicCD_Prototype:
            return {"assets/prototype", "rom/prototype", "builds/prototype"};
        case RomBuildVersion::SonicCD_Classic:
        case RomBuildVersion::SonicCD_1993:
            return {"assets/classic", "rom/classic", "builds/classic"};
        default:
            return {"assets", "rom", "builds"};
    }
}

} // namespace sclpr::resource
