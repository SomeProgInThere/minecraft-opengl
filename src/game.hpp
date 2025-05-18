#pragma once

#include "camera.hpp"
#include "texture_atlas.hpp"
#include "window.hpp"

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void update();

    unsigned int m_vertexBuffer{};
    unsigned int m_indexBuffer{};
    unsigned int m_vertexArray{};

    Window m_window;
    Camera m_camera;
    Shader m_quadShader;
    // Shader m_textShader;
    AtlasManager m_atlasManager;
};
