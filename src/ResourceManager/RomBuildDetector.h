#include "ResourceManager/RomBuildDetector.h"

#include <algorithm>
#include <cctype>

namespace sclpr::resource {

namespace {

std::string ToLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}

} // namespace

bool RomBuildDetector::IsSonicCdLike(const std::vector<uint8_t>& romData) {
    if (romData.size() < 512 * 1024) {
        return false;
    }

    const std::string header(reinterpret_cast<const char*>(romData.data()), std::min<size_t>(romData.size(), 8192ULL));
    const std::string lowered = ToLower(header);
    return lowered.find("sonic cd") != std::string::npos ||
           lowered.find("sega") != std::string::npos ||
           lowered.find("sonic") != std::string::npos;
}

RomBuildInfo RomBuildDetector::Detect(const std::vector<uint8_t>& romData, const std::string& fileName) {
    RomBuildInfo info;

    if (romData.empty()) {
        return info;
    }

    if (!IsSonicCdLike(romData)) {
        return info;
    }

    const std::string loweredFile = ToLower(fileName);
    const std::string header(reinterpret_cast<const char*>(romData.data()), std::min<size_t>(romData.size(), 4096ULL));
    const std::string loweredHeader = ToLower(header);

    if (loweredFile.find("2011") != std::string::npos ||
        loweredHeader.find("2011") != std::string::npos ||
        romData.size() >= 32 * 1024 * 1024) {
        info.buildVersion = RomBuildVersion::SonicCD_2011;
        info.displayName = "Sonic CD 2011";
        info.is2011Build = true;
        info.notes = "Detected 2011-era asset layout or larger payload footprint.";
        return info;
    }

    if (loweredFile.find("prototype") != std::string::npos ||
        loweredHeader.find("prototype") != std::string::npos) {
        info.buildVersion = RomBuildVersion::SonicCD_Prototype;
        info.displayName = "Sonic CD Prototype";
        info.notes = "Prototype or pre-release layout detected.";
        return info;
    }

    if (romData.size() < 16 * 1024 * 1024) {
        info.buildVersion = RomBuildVersion::SonicCD_1993;
        info.displayName = "Sonic CD 1993";
        info.notes = "Classic 1993-era layout detected.";
        return info;
    }

    info.buildVersion = RomBuildVersion::SonicCD_Classic;
    info.displayName = "Sonic CD Classic";
    info.notes = "Classic-genesis-alike layout detected.";
    return info;
}

bool RomBuildDetector::Is2011Build(const RomBuildInfo& info) noexcept {
    return info.is2011Build || info.buildVersion == RomBuildVersion::SonicCD_2011;
}

} // namespace sclpr::resource
