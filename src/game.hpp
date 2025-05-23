#pragma once

#include "system/player_camera.hpp"
#include "system/atlas_manager.hpp"
#include "opengl/window.hpp"
#include "world/chunk.hpp"

namespace minecraft {
    const std::filesystem::path SOURCE_DIR = PROJECT_SOURCE_DIR;
    const std::filesystem::path ASSETS_DIR = SOURCE_DIR / "assets";
    const std::filesystem::path SHADERS_DIR = SOURCE_DIR / "shaders";

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

        world::Chunk m_chunk;

        opengl::Window m_window;
        opengl::ShaderProgram m_quadShader;
        system::PlayerCamera m_camera;
        system::AtlasManager m_atlasManager;
    };
}
