#pragma once

#include <glm.hpp>

#include "direction.hpp"
#include "../systems/atlas_manager.hpp"

namespace minecraft::primitives {

    class Quad {
    public:
        Quad(Direction direction, glm::vec3 position);

        glm::vec3 Vertices[4]{};
        uint32_t DirectionID{};
        systems::AtlasRegion TextureRegion{};
    };
}
