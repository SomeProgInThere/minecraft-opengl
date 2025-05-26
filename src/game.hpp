#pragma once

#include "window.hpp"
#include "atlas_manager.hpp"
#include "chunk.hpp"

namespace minecraft {
    class Game {
    public:
        Game();
        ~Game();

        void run();

    private:
        void update();

        world::Chunk m_chunk;

        opengl::Window m_window;
        opengl::ShaderProgram m_renderProgram;
        system::PlayerCamera m_camera;
        system::AtlasManager m_atlasManager;
    };
}
