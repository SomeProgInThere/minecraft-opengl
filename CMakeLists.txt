cmake_minimum_required(VERSION 3.31)
project(minecraft_opengl)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(
        PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_library(
        glad STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/glad/src/glad.c
)
target_include_directories(
        glad PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/glad/include
)

add_library(glfw STATIC IMPORTED)
set_target_properties(
        glfw PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw/lib/glfw3.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw/include"
)

include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/glm"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/stb"
)

add_executable(minecraft_opengl src/main.cpp
        src/opengl/shader.hpp
        src/opengl/shader.cpp
        src/systems/atlas_manager.cpp
        src/systems/atlas_manager.hpp
        src/systems/player_camera.hpp
        src/systems/player_camera.cpp
        src/game.cpp
        src/game.hpp
        src/opengl/window.cpp
        src/opengl/window.hpp
        src/primitives/direction.hpp
        src/primitives/Quad.hpp
        src/primitives/chunk_mesh.hpp
        src/primitives/chunk_mesh.cpp
        src/primitives/quad.cpp
)

target_link_libraries(minecraft_opengl PRIVATE glad glfw)