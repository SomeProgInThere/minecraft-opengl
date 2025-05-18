#pragma once

#include "camera.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

constexpr unsigned int MIN_WINDOW_WIDTH = 800;
constexpr unsigned int MIN_WINDOW_HEIGHT = 600;

constexpr float DEFAULT_FPS_UPDATE_INTERVAL = 0.5f;

class Window {
public:
    Window(const std::string& title, unsigned int width, unsigned int height);
    ~Window();

    void update();
    void setCameraRefs(Camera& camera, Shader& shader);

    [[nodiscard]]
    GLFWwindow* getWindow() const;
    [[nodiscard]]
    float getAspectRatio() const;
    [[nodiscard]]
    bool getCursorLockedState() const;

private:
    void processInputs() const;
    void processToggleInputs();

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void mousePositionCallback(GLFWwindow* window, double posX, double posY);
    static void scrollCallback(GLFWwindow* window, double offsetX, double offsetY);

    GLFWwindow* m_window{};
    std::string m_title;

    Camera* m_camera{};
    Shader* m_shader{};

    unsigned int m_width{};
    unsigned int m_height{};

    float m_fps{};
    float m_frameCount{};
    float m_fpsTimer{};
    float m_lastFrameTime{};
    float m_deltaTime{};

    bool m_cursorLocked{};
    bool m_enableVsync{};
};
