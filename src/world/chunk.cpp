#include "chunk.hpp"

#include <glad/glad.h>
#include <iostream>

namespace minecraft::world {

    void Chunk::buildData() {

        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {

                    const auto position = glm::ivec3(x, y, z);
                    m_blocks[getBlockIndex(position)] = Block(BlockType::TEST);
                }
            }
        }
    }

    bool Chunk::buildMesh() {
        std::vector<primitive::Quad> quads{};

        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {

                    const glm::ivec3 position(x, y, z);
                    if (auto [current, left, down, back] = getAdjacentBlocks(position);
                        current->solid()) {

                        if (left->solid()) {
                            loadQuad(quads, primitive::Direction::LEFT, position);
                        }
                        if (down->solid()) {
                            loadQuad(quads, primitive::Direction::DOWN, position);
                        }
                        if (back->solid()) {
                            loadQuad(quads, primitive::Direction::BACK, position);
                        }
                    }
                    else {
                        if (!left->solid()) {
                            loadQuad(quads, primitive::Direction::RIGHT, position);
                        }
                        if (!down->solid()) {
                            loadQuad(quads, primitive::Direction::UP, position);
                        }
                        if (!back->solid()) {
                            loadQuad(quads, primitive::Direction::FRONT, position);
                        }
                    }
                }
            }
        }

        if (quads.empty()) {
            std::cerr << "No quads to render!" << std::endl;
            return false;
        }

        auto vertices = std::vector<primitive::Vertex>{};
        auto indices = std::vector<unsigned int>{};

        unsigned int index = 0;
        for (const auto& quad : quads) {
            for (int i = 0; i < 4; i++) {
                vertices.push_back(primitive::Vertex {
                    quad.vertices[i],
                    quad.texCoord[i],
                    primitive::getDirectionID(quad.direction),
                });
            }

            const auto nextIndices = {
                index, index + 1, index + 2,
                index, index + 2, index + 3,
            };

            indices.insert(indices.end(), nextIndices);
            index += 4;
        }

        return createBuffers(vertices, indices);
    }

    void Chunk::draw() const {
        glBindVertexArray(m_vertexArray);
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    const Block* Chunk::getBlock(glm::ivec3 position) const {
        while (position.x < 0) { position.x += 1; }
        while (position.x > CHUNK_SIZE - 1) { position.x -= 1; }
        while (position.y < 0) { position.y += 1; }
        while (position.y > CHUNK_SIZE - 1) { position.y -= 1; }
        while (position.z < 0) { position.z += 1; }
        while (position.z > CHUNK_SIZE - 1) { position.z -= 1; }

        return &m_blocks.at(getBlockIndex(position));
    }

    glm::ivec3 Chunk::getBlockPosition(const unsigned int index) {
        const glm::ivec3 position {
            static_cast<int>(static_cast<float>(index) / static_cast<float>(CHUNK_SIZE * CHUNK_SIZE)),
            static_cast<int>(std::fmodf(
                    static_cast<float>(index) / static_cast<float>(CHUNK_SIZE),
                    static_cast<float>(CHUNK_SIZE)
              )),
            static_cast<int>(std::fmodf(
                static_cast<float>(index),
                static_cast<float>(CHUNK_HEIGHT)
            )),
        };

        return position;
    }

    unsigned int Chunk::getBlockIndex(const glm::ivec3 position) {
        const unsigned int idx =  position.z | position.y << CHUNK_SIZE_BIT_OFFSET | position.x << (2 * CHUNK_SIZE_BIT_OFFSET);
        return idx;
    }

    Chunk::Chunk(const glm::ivec2 position)
        : m_position(position) {}

    Chunk::~Chunk() {
        glDeleteVertexArrays(1, &m_vertexArray);
        glDeleteBuffers(1, &m_vertexBuffer);
        glDeleteBuffers(1, &m_indexBuffer);
    }

    std::array<const Block*, 4> Chunk::getAdjacentBlocks(const glm::ivec3 position) const {
        const auto adjacentBlocks = std::array{
            getBlock(position),
            getBlock(glm::ivec3(position.x - 1, position.y, position.z)),  // left
            getBlock(glm::ivec3(position.x, position.y - 1, position.z)),  // down
            getBlock(glm::ivec3(position.x, position.y, position.z - 1)),  // back
        };

        return adjacentBlocks;
    }

    void Chunk::loadQuad(
        std::vector<primitive::Quad> &quads,
        const primitive::Direction direction,
        const glm::ivec3 position
    ) const {

        if (const Block* block = getBlock(position); !block->solid()) {
            return;
        }

        const auto quad = primitive::Quad{ direction, position };
        quads.push_back(quad);
    }

    bool Chunk::createBuffers(const std::vector<primitive::Vertex> &vertices, const std::vector<unsigned int> &indices) {
        glGenVertexArrays(1, &m_vertexArray);
        glGenBuffers(1, &m_vertexBuffer);
        glGenBuffers(1, &m_indexBuffer);

        glBindVertexArray(m_vertexArray);
        constexpr auto vertexSize = sizeof(primitive::Vertex);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * vertexSize, vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, nullptr);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(offsetof(primitive::Vertex, texCoord)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 1, GL_UNSIGNED_INT, GL_FALSE, vertexSize, reinterpret_cast<void*>(offsetof(primitive::Vertex, faceIndex)));

        glBindVertexArray(0);

        m_vertexCount = vertices.size();
        m_indexCount = indices.size();

        return m_indexCount != 0;
    }
}
