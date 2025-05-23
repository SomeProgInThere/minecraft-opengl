cmake_minimum_required(VERSION 3.20)
project(minecraft_opengl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

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
        IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw/lib/glfw3.lib
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw/include
)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/stb
)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(minecraft_opengl ${SOURCES})

target_link_libraries(minecraft_opengl PRIVATE glad glfw)