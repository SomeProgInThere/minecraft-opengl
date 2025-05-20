#include "shader.hpp"

#include "../game.hpp"

#include <glad/glad.h>
#include <gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace minecraft::opengl {

    ShaderProgram::ShaderProgram(const std::string_view vertexName, const std::string_view fragmentName) {
        const std::filesystem::path vertexPath = SHADERS_DIR / vertexName;
        const std::filesystem::path fragmentPath = SHADERS_DIR / fragmentName;

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

        Program = glCreateProgram();
        glAttachShader(Program, vertex);
        glAttachShader(Program, fragment);
        glLinkProgram(Program);

        glGetProgramiv(Program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(Program, sizeof(infoLog), nullptr, infoLog);
            std::cout << "Shader Linking Error:\n" << infoLog << std::endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void ShaderProgram::setUniformVec2(const std::string_view name, const glm::vec2 value) const {
        const int location = glGetUniformLocation(Program, name.data());
        glUniform2f(location, value.x, value.y);
    }

    void ShaderProgram::setUniformMat4(const std::string_view name, glm::mat4 value) const {
        const int location = glGetUniformLocation(Program, name.data());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void ShaderProgram::setUniformInt(const std::string_view name, const int value) const {
        const int location = glGetUniformLocation(Program, name.data());
        glUniform1i(location, value);
    }

    void ShaderProgram::use() const {
        glUseProgram(Program);
    }
}
