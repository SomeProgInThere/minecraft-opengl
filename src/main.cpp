#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "texture_atlas.hpp"
#include "camera.hpp"

#include <iostream>

float vertices[] = {
    // Position        TexCoords
    0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top right
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom left
   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,   // top left
};

unsigned int indices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

float deltaTime = 0.0f;
float lastFrameTime = 0.0f;

constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

Camera camera({ 0.0f, 0.0f, 3.0f }, SCR_WIDTH / static_cast<float>(SCR_HEIGHT));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool cursorLocked = false;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow *window);
void toggleCursorLock(GLFWwindow* window);

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSetWindowSizeLimits(window, SCR_WIDTH, SCR_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    const Shader program("vertex.glsl", "frag.glsl");

    unsigned int vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    unsigned int vertexBuffer;
    glGenBuffers(1, &vertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    AtlasManager manager;
    manager.addTexture("test0", "test.png");
    manager.addTexture("test1", "test1.png");
    manager.addTexture("test2", "test2.png");
    manager.addTexture("test3", "test3.png");

    if (!manager.build()) {
        std::cout << "Failed to build texture atlas" << std::endl;
        return -1;
    }

    if (!manager.save("atlas.png")) {
        std::cout << "Failed to save texture atlas" << std::endl;
    }

    glBindTexture(GL_TEXTURE_2D, manager.getID());
    glEnable(GL_DEPTH_TEST);


    auto model = glm::mat4(1.0f);

    while (!glfwWindowShouldClose(window)) {
        const float currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        processInput(window);
        toggleCursorLock(window);

        glClearColor(0.4f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.use();
        program.setUniformMat4("model", model);

        if (cursorLocked) {
            camera.updateUniforms(program);
        }

        const AtlasRegion* region = manager.getRegion("test0");

        program.setUniformVec2("texOffset", region->topLeft);
        program.setUniformVec2("texScale", {
            region->bottomRight.x - region->topLeft.x,
            region->bottomRight.y - region->topLeft.y
        });

        glBindVertexArray(vertexArray);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    manager.unloadAll();

    glDeleteVertexArrays(1, &vertexArray);
    glDeleteBuffers(1, &indexBuffer);
    glDeleteBuffers(1, &vertexBuffer);

    glfwTerminate();
    return 0;
}

void toggleCursorLock(GLFWwindow* window) {
    if (cursorLocked) {
        camera.saveOrientation(false);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        camera.saveOrientation(true);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void framebufferSizeCallback(GLFWwindow*, const int width, const int height) {
    glViewport(0, 0, width, height);
    const float aspectRatio = width / static_cast<float>(height);
    camera.aspectRatio = aspectRatio;
}

void processInput(GLFWwindow *window) {
    static bool escapePressed = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (!escapePressed)
            cursorLocked = !cursorLocked;
        escapePressed = true;
    } else {
        escapePressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKeyboard(UP, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.processKeyboard(DOWN, deltaTime);
}

void mouseCallback(GLFWwindow*, const double xPos, const double yPos) {
    const auto xpos = static_cast<float>(xPos);
    const auto ypos = static_cast<float>(yPos);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    const float xoffset = xpos - lastX;
    const float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow*, double, const double yOffset) {
    camera.processMouseScroll(static_cast<float>(yOffset));
}
