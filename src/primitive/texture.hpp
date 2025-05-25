#pragma once

#include <glm.hpp>

#include <memory>
#include <string>

namespace minecraft::primitive {
    struct Texture {
        int channels;
        int width;
        int height;
        std::unique_ptr<unsigned char> data;
        std::string path;
    };

    struct AtlasRegion {
        int width;
        int height;
        glm::vec2 topLeft;
        glm::vec2 bottomRight;

        [[nodiscard]]
        glm::vec2 getPosition() const {
            return topLeft;
        }

        [[nodiscard]]
        glm::vec2 getScale() const {
            return { bottomRight.x - topLeft.x, bottomRight.y - topLeft.y };
        }
    };
}
