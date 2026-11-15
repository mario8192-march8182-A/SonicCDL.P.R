#pragma once

#include <array>
#include <glm/glm.hpp>
#include <string>

namespace sclpr::renderer {

struct SpriteFrame {
    int x{0};
    int y{0};
    int width{16};
    int height{16};
};

struct Sprite {
    std::string textureName;
    std::string atlasName;

    glm::vec2 position{0.0f, 0.0f};
    glm::vec2 size{16.0f, 16.0f};
    glm::vec2 origin{0.0f, 0.0f};
    glm::vec2 scale{1.0f, 1.0f};

    float rotation{0.0f};
    glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
    bool visible{true};

    int frameIndex{0};
    std::array<int, 4> sourceRect{0, 0, 16, 16};
};

} // namespace sclpr::renderer
