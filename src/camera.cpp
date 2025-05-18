#include "camera.hpp"

Camera::Camera(const glm::vec3 position, const float aspectRatio) {
    Position = position;
    AspectRatio = aspectRatio;
    Fov = DEFAULT_FOV;
    MovementSpeed = DEFAULT_SPEED;
    MouseSensitivity = DEFAULT_SENSITIVITY;

    m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    m_right = glm::vec3(1.0f, 0.0f, 0.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);

    m_yaw = DEFAULT_YAW;
    m_pitch = DEFAULT_PITCH;

    updateVectors();
}

void Camera::updateUniforms(const Shader& shader) const {
    const auto view = glm::lookAt(Position, Position + m_front, m_up);
    const auto projection = glm::perspective(glm::radians(Fov), AspectRatio, DEFAULT_Z_NEAR, DEFAULT_Z_FAR);

    shader.setUniformMat4("view", view);
    shader.setUniformMat4("projection", projection);
}

void Camera::processKeyboard(const CameraMovement direction, const float deltaTime) {
    const float velocity = MovementSpeed * deltaTime;

    switch (direction) {
        case FORWARD:
            Position += m_front * velocity;
            break;
        case BACKWARD:
            Position -= m_front * velocity;
            break;
        case LEFT:
            Position -= m_right * velocity;
            break;
        case RIGHT:
            Position += m_right * velocity;
            break;
        case UP:
            Position += m_up * velocity;
            break;
        case DOWN:
            Position -= m_up * velocity;
            break;
    }
}

void Camera::processMouseMovement(const float offsetX, const float offsetY) {
    m_yaw += offsetX * MouseSensitivity;
    m_pitch += offsetY * MouseSensitivity;

    if (m_pitch > DEFAULT_MAX_PITCH)
        m_pitch = DEFAULT_MAX_PITCH;
    if (m_pitch < -DEFAULT_MAX_PITCH)
        m_pitch = -DEFAULT_MAX_PITCH;

    updateVectors();
}

void Camera::processMouseScroll(const float yOffset) {
    Fov -= yOffset;

    if (Fov < 1.0f)
        Fov = 1.0f;
    if (Fov > DEFAULT_MAX_FOV)
        Fov = DEFAULT_MAX_FOV;
}

void Camera::updateVectors() {
    glm::vec3 front{};

    front.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    front.y = glm::sin(glm::radians(m_pitch));
    front.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));

    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, glm::vec3(0.0f, 1.0f, 0.0f)));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
