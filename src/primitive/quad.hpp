#pragma once

#include <glm.hpp>

#include "direction.hpp"
#include "../system/atlas_manager.hpp"

namespace minecraft::primitive {
    struct Quad {
        glm::vec3 vertices[4]{};
        Direction direction{};
        system::AtlasRegion textureRegion{};

        Quad(const Direction direction, const glm::vec3 position, const system::AtlasRegion& textureRegion)
            : direction(direction),
            textureRegion(textureRegion) {

            switch (direction) {
                case Direction::FRONT:
                    vertices[0] = glm::vec3(position.x + 1.0, position.y, position.z);
                    vertices[1] = glm::vec3(position.x + 1.0, position.y + 1.0, position.z);
                    vertices[2] = glm::vec3(position.x, position.y + 1.0, position.z);
                    vertices[3] = glm::vec3(position.x, position.y, position.z);
                    break;

                case Direction::BACK:
                    vertices[0] = glm::vec3(position.x, position.y, position.z);
                    vertices[1] = glm::vec3(position.x, position.y + 1.0, position.z);
                    vertices[2] = glm::vec3(position.x + 1.0, position.y + 1.0, position.z);
                    vertices[3] = glm::vec3(position.x + 1.0, position.y, position.z);
                    break;

                case Direction::LEFT:
                    vertices[0] = glm::vec3(position.x, position.y, position.z);
                    vertices[1] = glm::vec3(position.x, position.y, position.z + 1.0);
                    vertices[2] = glm::vec3(position.x, position.y + 1.0, position.z + 1.0);
                    vertices[3] = glm::vec3(position.x, position.y + 1.0, position.z);
                    break;

                case Direction::RIGHT:
                    vertices[0] = glm::vec3(position.x, position.y + 1.0, position.z);
                    vertices[1] = glm::vec3(position.x, position.y + 1.0, position.z + 1.0);
                    vertices[2] = glm::vec3(position.x, position.y, position.z + 1.0);
                    vertices[3] = glm::vec3(position.x, position.y, position.z);
                    break;

                case Direction::UP:
                    vertices[0] = glm::vec3(position.x, position.y, position.z + 1.0);
                    vertices[1] = glm::vec3(position.x + 1.0, position.y, position.z + 1.0);
                    vertices[2] = glm::vec3(position.x + 1.0, position.y, position.z);
                    vertices[3] = glm::vec3(position.x, position.y, position.z);
                    break;

                case Direction::DOWN:
                    vertices[0] = glm::vec3(position.x, position.y, position.z);
                    vertices[1] = glm::vec3(position.x + 1.0, position.y, position.z);
                    vertices[2] = glm::vec3(position.x + 1.0, position.y, position.z + 1.0);
                    vertices[3] = glm::vec3(position.x, position.y, position.z + 1.0);
                    break;
            }
        }
    };
}
