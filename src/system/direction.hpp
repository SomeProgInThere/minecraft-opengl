#pragma once

#include <glm.hpp>

namespace minecraft::system {

    enum class Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN,
    };

    constexpr glm::vec3 getNormal(const Direction direction) {
        switch (direction) {
            case Direction::FORWARD:  return { 0,  0,  1 };
            case Direction::BACKWARD: return { 0,  0, -1 };
            case Direction::LEFT:     return {-1,  0,  0 };
            case Direction::RIGHT:    return { 1,  0,  0 };
            case Direction::UP:       return { 0,  1,  0 };
            case Direction::DOWN:     return { 0, -1,  0 };

            default:
                return { 0, 0, 0 };
        }
    }
}
