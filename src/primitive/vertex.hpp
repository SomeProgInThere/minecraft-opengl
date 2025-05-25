#pragma once

#include <glm.hpp>

namespace minecraft::primitive {

    struct Vertex {
        glm::vec3 position;
        unsigned int normalIndex;
        unsigned int blockIndex;
    };
}
