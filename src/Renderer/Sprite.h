#pragma once

#include <glm/glm.hpp>
#include <string>

namespace sclpr::renderer {

struct Sprite {
    float x{0.0f};
    float y{0.0f};
    float width{16.0f};
    float height{16.0f};
    float scaleX{1.0f};
    float scaleY{1.0f};
    float rotation{0.0f};
    glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
    bool visible{true};
};

} // namespace sclpr::renderer
