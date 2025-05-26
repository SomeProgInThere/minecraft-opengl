#pragma once

namespace minecraft::primitive {

    enum class Direction {
        UP,
        DOWN,
        RIGHT,
        LEFT,
        FRONT,
        BACK,
    };

    inline unsigned int getDirectionID(Direction direction) {
        return static_cast<unsigned int>(direction);
    }
}
