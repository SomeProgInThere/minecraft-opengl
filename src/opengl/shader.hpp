#pragma once

#include <glm.hpp>
#include <string_view>

namespace minecraft::opengl {

    class ShaderProgram {
    public:
        ShaderProgram(std::string_view vertexName, std::string_view fragmentName);

        void use() const;
        void setUniformVec2(std::string_view name, glm::vec2 value) const;
        void setUniformMat4(std::string_view name, glm::mat4 value) const;
        void setUniformInt(std::string_view name, int value) const;

        unsigned int Program{};
    };
}
