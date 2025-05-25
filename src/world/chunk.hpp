#pragma once

#include "../primitive/vertex.hpp"
#include "../primitive/direction.hpp"
#include "../primitive/quad.hpp"
#include "../opengl/shader.hpp"
#include "block.hpp"

#include <glm.hpp>

#include <array>
#include <vector>

namespace minecraft::world {
    constexpr unsigned int CHUNK_SIZE = 16;
    constexpr unsigned int CHUNK_HEIGHT = 128;
    constexpr unsigned int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT;

    const int CHUNK_SHIFT_OFFSET = static_cast<int>(std::log2(static_cast<int>(CHUNK_SIZE)));

    class Chunk {
    public:
        explicit Chunk(glm::ivec2 position);
        ~Chunk();

        void buildData();
        bool buildMesh();
        void draw() const;

    private:
        [[nodiscard]]
        const Block* getBlock(glm::ivec3 position) const;
        [[nodiscard]]
        std::array<const Block*, 4> getAdjacentBlocks(glm::ivec3 position) const;

        static unsigned int getBlockIndex(glm::ivec3 position);
        static glm::ivec3 getBlockPosition(unsigned int index);

        void loadQuad(
            std::vector<primitive::Quad>& quads, primitive::Direction direction, glm::ivec3 position
        ) const;
        bool createBuffers(const std::vector<primitive::Vertex>& vertices, const std::vector<unsigned int>& indices);

        glm::ivec2 m_position{};
        std::array<Block, CHUNK_VOLUME> m_blocks{};

        std::vector<primitive::Vertex> m_vertices{};
        std::vector<unsigned int> m_indices{};

        unsigned int m_vertexBuffer{};
        unsigned int m_indexBuffer{};
        unsigned int m_vertexArray{};

        unsigned int m_vertexCount{};
        unsigned int m_indexCount{};
    };
}
