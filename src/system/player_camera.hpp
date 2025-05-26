#pragma once

#include "shader.hpp"
#include "direction.hpp"

namespace minecraft::system {

    constexpr float DEFAULT_FOV = 45.0f;
    constexpr float DEFAULT_MAX_FOV = 80.0f;

    constexpr float DEFAULT_SPEED = 2.5f;
    constexpr float DEFAULT_SENSITIVITY = 0.06f;

    constexpr float DEFAULT_PITCH = 0.0f;
    constexpr float DEFAULT_MAX_PITCH = 89.0f;

    constexpr float DEFAULT_YAW = -90.0f;
    constexpr float DEFAULT_Z_NEAR = 0.1f;
    constexpr float DEFAULT_Z_FAR = 100.0f;

    class PlayerCamera {
    public:
        PlayerCamera(glm::vec3 position, float aspectRatio);

        void updateUniforms(const opengl::ShaderProgram& shader) const;

        void processKeyboard(primitive::Direction direction, float deltaTime);
        void processMouseMovement(float offsetX, float offsetY);
        void processMouseScroll(float yOffset);

        glm::vec3 Position{};

        float AspectRatio;
        float Fov;
        float MovementSpeed;
        float MouseSensitivity;

    private:
        void updateVectors();

        glm::vec3 m_front{};
        glm::vec3 m_right{};
        glm::vec3 m_up{};

        float m_yaw;
        float m_pitch;
    };
}
