#include "shader.hpp"

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

Shader::Shader(const char* vertexName, const char* fragmentName) {
    const std::filesystem::path projectDir = PROJECT_SOURCE_DIR;
    const std::filesystem::path vertexPath = projectDir / "shaders" / vertexName;
    const std::filesystem::path fragmentPath = projectDir / "shaders" / fragmentName;

    std::string vertexProgram;
    std::string fragmentProgram;

    std::ifstream vertexFile;
    std::ifstream fragmentFile;

    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);

        std::stringstream vertexStream, fragmentStream;
        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();

        vertexFile.close();
        fragmentFile.close();

        vertexProgram = vertexStream.str();
        fragmentProgram = fragmentStream.str();
    }
    catch (std::ifstream::failure &err) {
        std::cout << "Shader Loading Error: " << err.what() << std::endl;
    }

    const char* vertexStr = vertexProgram.c_str();
    const char* fragmentStr = fragmentProgram.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexStr, nullptr);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, sizeof(infoLog), nullptr, infoLog);
        std::cout << "Vertex Shader Compilation Error:\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentStr, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, sizeof(infoLog), nullptr, infoLog);
        std::cout << "Fragment Shader Compilation Error:\n" << infoLog << std::endl;
    }

    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        std::cout << "Shader Linking Error:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::setUniformv2f(const char *name, const glm::vec2 value) const {
    const int location = glGetUniformLocation(program, name);
    glUniform2f(location, value.x, value.y);
}

void Shader::use() const {
    glUseProgram(program);
}