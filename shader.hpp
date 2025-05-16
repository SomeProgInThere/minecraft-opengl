#pragma once

#include "vec2.hpp"

class Shader {
public:
    Shader(const char* vertexName, const char* fragmentName);
    void use() const;
    void setUniformv2f(const char* name, glm::vec2 value) const;

    unsigned int program;
};
