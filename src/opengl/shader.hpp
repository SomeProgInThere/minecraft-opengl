#pragma once

#include <filesystem>
#include <glm.hpp>

namespace minecraft::opengl {
    const std::filesystem::path SOURCE_DIR = PROJECT_SOURCE_DIR;
    const std::filesystem::path SHADERS_DIR = SOURCE_DIR / "shaders";

    class ShaderProgram {
    public:
        ShaderProgram(std::string_view vertexName, std::string_view fragmentName);

        void use() const;
        void setUniformMat4(std::string_view name, glm::mat4 value) const;

        unsigned int Program{};
    };
}
