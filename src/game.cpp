#include "game.hpp"

#include <iostream>

namespace minecraft {
    Game::Game()
        : m_window(opengl::Window("minecraft-opengl", 1280, 720)),
        m_quadShader(opengl::ShaderProgram("quad_vertex.glsl", "quad_fragment.glsl")),
        m_camera(system::PlayerCamera(glm::vec3(0.0f, 0.0f, 3.0f), m_window.getAspectRatio())),
        m_chunk(glm::ivec2(0)) {

        m_window.setCameraRefs(m_camera, m_quadShader);

        m_atlasManager = system::AtlasManager();
        if (!m_atlasManager.loadAssetDirectory()) {
            std::cout << "Failed to load texture atlas!" << std::endl;
            return;
        }

        if (!m_atlasManager.build()) {
            std::cout << "Failed to build texture atlas!" << std::endl;
            return;
        }

        glBindTexture(GL_TEXTURE_2D, m_atlasManager.getID());

        m_chunk.buildData();
        m_chunk.buildMesh(m_atlasManager);
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
        glClearColor(0.2f, 0.227f, 0.251f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_chunk.draw(m_quadShader);
        m_window.update();
    }
}
