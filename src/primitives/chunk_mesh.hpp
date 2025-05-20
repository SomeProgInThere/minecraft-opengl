#pragma once

#include <glm.hpp>
#include <vector>

namespace minecraft::primitives {

    struct Vertex {
        glm::vec3 position;
    };

    class ChunkMesh {
    public:
        ChunkMesh();
        ~ChunkMesh();

        void build();
        void draw();

    private:
        std::vector<Vertex> m_vertices{};
        std::vector<unsigned int> m_indices{};

        unsigned int m_vertexBuffer{};
        unsigned int m_indexBuffer{};
        unsigned int m_vertexArray{};
    };
}
