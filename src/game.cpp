#include "game.hpp"

#include <iostream>

namespace minecraft {

    constexpr float VERTICES[] = {
        // Position        TexCoords
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,   // top left
    };

    constexpr unsigned int INDICES[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    Game::Game()
        : m_window(opengl::Window("minecraft-opengl", 1280, 720)),
        m_quadShader(opengl::ShaderProgram("quadVertex.glsl", "quadFragment.glsl")),
        m_camera(systems::PlayerCamera(glm::vec3(0.0f, 0.0f, 3.0f), m_window.getAspectRatio())) {

        glGenVertexArrays(1, &m_vertexArray);
        glBindVertexArray(m_vertexArray);

        glGenBuffers(1, &m_vertexBuffer);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &m_indexBuffer);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);

        m_window.setCameraRefs(m_camera, m_quadShader);

        m_atlasManager = systems::AtlasManager();
        m_atlasManager.addTexture("test0", "test.png");
        m_atlasManager.addTexture("test1", "test1.png");
        m_atlasManager.addTexture("test2", "test2.png");
        m_atlasManager.addTexture("test3", "test3.png");

        if (!m_atlasManager.build()) {
            std::cout << "Failed to build texture atlas" << std::endl;
        }

        glBindTexture(GL_TEXTURE_2D, m_atlasManager.getID());
    }

    Game::~Game() {
        m_atlasManager.unloadAll();

        glDeleteVertexArrays(1, &m_vertexArray);
        glDeleteBuffers(1, &m_vertexBuffer);
        glDeleteBuffers(1, &m_indexBuffer);

        glfwTerminate();
    }

    void Game::run() {
        while (!glfwWindowShouldClose(m_window.getWindow())) {
            update();
        }
    }

    void Game::update() {
        glClearColor(0.4f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_quadShader.use();
        m_quadShader.setUniformMat4("model", glm::mat4(1.0f));

        const auto region = m_atlasManager.getRegion("test0").value();

        m_quadShader.setUniformVec2("texOffset", region.topLeft);
        m_quadShader.setUniformVec2("texScale", {
            region.bottomRight.x - region.topLeft.x,
            region.bottomRight.y - region.topLeft.y
        });

        glBindVertexArray(m_vertexArray);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        m_window.update();
    }
}
