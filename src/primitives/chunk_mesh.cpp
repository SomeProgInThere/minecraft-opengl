#include "chunk_mesh.hpp"

#include <glad/glad.h>

namespace minecraft::primitives {

    ChunkMesh::ChunkMesh() = default;

    ChunkMesh::~ChunkMesh() {
        glDeleteVertexArrays(1, &m_vertexArray);
        glDeleteBuffers(1, &m_vertexBuffer);
        glDeleteBuffers(1, &m_indexBuffer);
    }
}
