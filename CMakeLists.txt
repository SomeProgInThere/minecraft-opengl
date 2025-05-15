cmake_minimum_required(VERSION 3.31)
project(minecraft_opengl)

set(CMAKE_CXX_STANDARD 20)

# Create GLAD as a static library
add_library(
        glad STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/glad/src/glad.c
)
target_include_directories(
        glad PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/glad/include
)

# Main executable
add_executable(minecraft_opengl main.cpp)

# GLFW setup
add_library(glfw STATIC IMPORTED)
set_target_properties(
        glfw PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw/lib/glfw3.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw/include"
)

# Link dependencies to main executable
target_link_libraries(minecraft_opengl PRIVATE glad glfw)