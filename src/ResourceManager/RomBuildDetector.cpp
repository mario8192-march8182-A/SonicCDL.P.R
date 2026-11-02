#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace sclpr::resource {

enum class RomBuildVersion {
    Unknown,
    SonicCD_1993,
    SonicCD_2011,
    SonicCD_Prototype,
    SonicCD_Classic
};

struct RomBuildInfo {
    RomBuildVersion buildVersion{RomBuildVersion::Unknown};
    std::string displayName{"Unknown"};
    bool is2011Build{false};
    std::string notes;
};

class RomBuildDetector {
public:
    static RomBuildInfo Detect(const std::vector<uint8_t>& romData, const std::string& fileName = "");
    static bool Is2011Build(const RomBuildInfo& info) noexcept;
    static bool IsSonicCdLike(const std::vector<uint8_t>& romData);
};

} // namespace sclpr::resource
