#include "camera.hpp"

Camera::Camera(const glm::vec3 position, const float aspectRatio)
    : position(position),
    aspectRatio(aspectRatio),
    fov(DEFAULT_FOV),
    movementSpeed(DEFAULT_SPEED),
    mouseSensitivity(DEFAULT_SENSITIVITY),
    m_front({ 0.0f, 0.0f, -1.0f }),
    m_right({ 1.0f, 0.0f, 0.0f }),
    m_up({ 0.0f, 1.0f, 0.0f }),
    m_yaw(DEFAULT_YAW),
    m_pitch(DEFAULT_PITCH),
    m_savedYaw(m_yaw),
    m_savedPitch(m_pitch) {

    updateVectors();
}

void Camera::updateUniforms(const Shader &program) const {
    const auto view = glm::lookAt(position, position + m_front, m_up);
    const auto projection = glm::perspective(glm::radians(fov), aspectRatio, DEFAULT_Z_NEAR, DEFAULT_Z_FAR);

    program.setUniformMat4("view", view);
    program.setUniformMat4("projection", projection);
}

void Camera::saveOrientation(const bool useSaved) {
    if (useSaved) {
        m_yaw = m_savedYaw;
        m_pitch = m_savedPitch;
    } else {
        m_savedYaw = m_yaw;
        m_savedPitch = m_pitch;
    }
}

void Camera::processKeyboard(const CameraMovement direction, const float deltaTime) {
    const float velocity = movementSpeed * deltaTime;

    switch (direction) {
        case FORWARD:
            position += m_front * velocity;
            break;
        case BACKWARD:
            position -= m_front * velocity;
            break;
        case LEFT:
            position -= m_right * velocity;
            break;
        case RIGHT:
            position += m_right * velocity;
            break;
        case UP:
            position += m_up * velocity;
            break;
        case DOWN:
            position -= m_up * velocity;
            break;
    }
}

void Camera::processMouseMovement(const float xOffset, const float yOffset) {
    m_yaw += xOffset * mouseSensitivity;
    m_pitch += yOffset * mouseSensitivity;

    if (m_pitch > DEFAULT_MAX_PITCH)
        m_pitch = DEFAULT_MAX_PITCH;
    if (m_pitch < -DEFAULT_MAX_PITCH)
        m_pitch = -DEFAULT_MAX_PITCH;

    updateVectors();
}

void Camera::processMouseScroll(const float yOffset) {
    fov -= yOffset;

    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > DEFAULT_MAX_FOV)
        fov = DEFAULT_MAX_FOV;
}

void Camera::updateVectors() {
    glm::vec3 front = {
        glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch)),
        glm::sin(glm::radians(m_pitch)),
        glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch))
    };

    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, { 0.0f, 1.0f, 0.0f }));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
