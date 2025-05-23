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

    inline unsigned int getDirectionID(Direction direction) {
        return static_cast<unsigned int>(direction);
    }
}
