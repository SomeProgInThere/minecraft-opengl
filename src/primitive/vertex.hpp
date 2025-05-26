#pragma once

#include <glm.hpp>

namespace minecraft::primitive {

    struct Vertex {
        glm::vec3 position;
        glm::vec2 texCoord;
        unsigned int faceIndex;
    };
}
