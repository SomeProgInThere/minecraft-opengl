#pragma once

#include "system/player_camera.hpp"
#include "system/atlas_manager.hpp"
#include "opengl/window.hpp"

namespace minecraft {

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

        opengl::Window m_window;
        opengl::ShaderProgram m_quadShader;
        system::PlayerCamera m_camera;
        system::AtlasManager m_atlasManager;
    };
}
