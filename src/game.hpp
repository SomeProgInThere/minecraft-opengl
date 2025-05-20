#pragma once

#include "systems/player_camera.hpp"
#include "systems/atlas_manager.hpp"
#include "opengl/window.hpp"

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

        opengl::Window m_window;
        opengl::ShaderProgram m_quadShader;
        systems::PlayerCamera m_camera;
        systems::AtlasManager m_atlasManager;
    };
}
