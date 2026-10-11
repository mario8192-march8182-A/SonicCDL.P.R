#pragma once

#include <string>
#include <vector>

namespace sclpr::game {

struct TransformationSpec {
    std::string id;
    std::string displayName;
    std::string owner;
    std::string description;
    float speedMultiplier{1.0f};
    float jumpMultiplier{1.0f};
    bool enabled{true};
};

class GemstoneSonicController {
public:
    GemstoneSonicController() = default;
    ~GemstoneSonicController() = default;

    void Activate();
    void Deactivate();
    bool IsActive() const noexcept;
    const TransformationSpec& GetSpec() const noexcept;

private:
    bool m_active{false};
    TransformationSpec m_spec{
        "gemstone_sonic",
        "Gemstone Sonic",
        "SCDLPR",
        "A transformation inspired by the Time Stones and the prototype-era identity of the project.",
        1.5f,
        1.2f,
        true
    };
};

} // namespace sclpr::game
