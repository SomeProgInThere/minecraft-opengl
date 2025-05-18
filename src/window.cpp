#include "window.hpp"

#include <format>
#include <iostream>

Window::Window(const std::string &title, const unsigned int width, const unsigned int height)
    : m_width(width), m_height(height), m_title(title) {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window) {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD!" << std::endl;
    }

    glViewport(0, 0, m_width, m_height);
    glfwSetWindowSizeLimits(m_window, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glfwSetWindowUserPointer(m_window, this);

    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    glfwSetCursorPosCallback(m_window, mousePositionCallback);
    glfwSetScrollCallback(m_window, scrollCallback);

    glEnable(GL_DEPTH_TEST);
}

Window::~Window() {
    glfwDestroyWindow(m_window);
}

void Window::update() {
    if (m_cursorLocked) {
        m_camera->updateUniforms(*m_shader);
    }

    processInputs();
    processToggleInputs();

    const float currFrameTime = glfwGetTime();
    m_deltaTime = currFrameTime - m_lastFrameTime;
    m_lastFrameTime = currFrameTime;

    m_frameCount++;
    m_fpsTimer += m_deltaTime;

    if (m_fpsTimer >= DEFAULT_FPS_UPDATE_INTERVAL) {
        m_fps = m_frameCount / m_fpsTimer;
        m_frameCount = 0;
        m_fpsTimer = 0.0f;

        const std::string title = std::format("{} | FPS: {}", m_title, static_cast<int>(m_fps));
        glfwSetWindowTitle(m_window, title.c_str());
    }

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

GLFWwindow *Window::getWindow() const {
    return m_window;
}

void Window::setCameraRefs(Camera &camera, Shader& shader) {
    m_camera = &camera;
    m_shader = &shader;
}

float Window::getAspectRatio() const {
    return m_width / static_cast<float>(m_height);
}

bool Window::getCursorLockedState() const {
    return m_cursorLocked;
}

void Window::processInputs() const {
    if (glfwGetKey(m_window, GLFW_KEY_X) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);

    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera->processKeyboard(FORWARD, m_deltaTime);

    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera->processKeyboard(BACKWARD, m_deltaTime);

    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera->processKeyboard(LEFT, m_deltaTime);

    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera->processKeyboard(RIGHT, m_deltaTime);

    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_camera->processKeyboard(UP, m_deltaTime);

    if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_camera->processKeyboard(DOWN, m_deltaTime);
}

void Window::processToggleInputs() {
    static bool escapePressed = false;
    static bool keyVPressed = false;

    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (!escapePressed) {
            m_cursorLocked = !m_cursorLocked;
            glfwSetInputMode(m_window, GLFW_CURSOR, m_cursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        }

        escapePressed = true;
    } else {
        escapePressed = false;
    }

    if (glfwGetKey(m_window, GLFW_KEY_V) == GLFW_PRESS) {
        if (!keyVPressed && m_cursorLocked) {
            m_enableVsync = !m_enableVsync;
            glfwSwapInterval(m_enableVsync ? 1 : 0);
        }

        keyVPressed = true;
    } else {
        keyVPressed = false;
    }
}

void Window::framebufferSizeCallback(GLFWwindow* window, const int width, const int height) {
    const auto* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));

    glViewport(0, 0, width, height);
    const float aspectRatio = width / static_cast<float>(height);
    windowPtr->m_camera->AspectRatio = aspectRatio;
}

void Window::mousePositionCallback(GLFWwindow *window, const double posX, const double posY) {
    const auto* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));

    static bool firstMove = true;
    static float lastX = windowPtr->m_width / 2.0f;
    static float lastY = windowPtr->m_height / 2.0f;

    const auto fposX = static_cast<float>(posX);
    const auto fposY = static_cast<float>(posY);

    if (firstMove) {
        lastX = fposX;
        lastY = fposY;
        firstMove = false;
    }

    const float offsetX = fposX - lastX;
    const float offsetY = lastY - fposY;

    lastX = fposX;
    lastY = fposY;

    if (windowPtr->m_cursorLocked) {
        windowPtr->m_camera->processMouseMovement(offsetX, offsetY);
    }
}

void Window::scrollCallback(GLFWwindow *window, const double, const double offsetY) {
    const auto* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
    windowPtr->m_camera->processMouseScroll(static_cast<float>(offsetY));
}