#pragma once

#include "../system/player_camera.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace minecraft::opengl {

    constexpr unsigned int MIN_WINDOW_HEIGHT = 600;
    constexpr unsigned int MIN_WINDOW_WIDTH = 800;

    constexpr float DEFAULT_FPS_UPDATE_INTERVAL = 0.5f;

    class Window {
    public:
        Window(std::string_view title, int width, int height);
        ~Window();

        void update();
        void setCameraRefs(system::PlayerCamera& camera, ShaderProgram& shader);

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
        std::string_view m_title;

        system::PlayerCamera* m_camera{};
        ShaderProgram* m_shader{};

        int m_width{};
        int m_height{};

        float m_fps{};
        float m_frameCount{};
        float m_fpsTimer{};
        float m_lastFrameTime{};
        float m_deltaTime{};

        bool m_cursorLocked{};
        bool m_enableVsync{};
    };
}
