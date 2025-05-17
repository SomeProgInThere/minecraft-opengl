#pragma once

#include <glm.hpp>

class Shader {
public:
    Shader(const char* vertexName, const char* fragmentName);
    void use() const;
    void setUniformVec2(const char* name, glm::vec2 value) const;
    void setUniformMat4(const char* name, glm::mat4 value) const;

    unsigned int program{};
};
