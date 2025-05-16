#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "texture_atlas.hpp"

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

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Hello World", nullptr, nullptr);
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

    glViewport(0, 0, 800, 800);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, const int width, const int height) {
        glViewport(0, 0, width, height);
    });

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

    TextureAtlas manager;
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

    while (!glfwWindowShouldClose(window)) {

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        glClearColor(0.4f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        program.use();

        const AtlasRegion* region = manager.getRegion("test1");

        program.setUniformv2f("texOffset", region->topLeft);
        program.setUniformv2f("texScale", {
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

    glfwTerminate();
    return 0;
}