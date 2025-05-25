#pragma once

namespace minecraft::primitive {

    enum class Direction {
        FRONT,
        BACK,
        LEFT,
        RIGHT,
        UP,
        DOWN,
    };

    inline uint8_t getDirectionID(Direction direction) {
        return static_cast<uint8_t>(direction);
    }
}
