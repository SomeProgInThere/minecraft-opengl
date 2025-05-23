#include "chunk.hpp"

#include <iostream>
#include <glad/glad.h>

#include "../primitive/quad.hpp"

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

    bool Chunk::buildMesh(system::AtlasManager& manager) {
        std::vector<primitive::Quad> quads{};

        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {

                    const glm::ivec3 position(x, y, z);
                    if (auto [current, left, down, back] = getAdjacentBlocks(position);
                        current->solid()) {

                        if (!left->solid()) {
                            loadQuad(quads, manager, primitive::Direction::LEFT, position);
                        }
                        if (!down->solid()) {
                            loadQuad(quads, manager, primitive::Direction::DOWN, position);
                        }
                        if (!back->solid()) {
                            loadQuad(quads, manager, primitive::Direction::BACK, position);
                        }
                    }
                    else {
                        if (left->solid()) {
                            loadQuad(quads, manager, primitive::Direction::RIGHT, position);
                        }
                        if (down->solid()) {
                            loadQuad(quads, manager, primitive::Direction::UP, position);
                        }
                        if (back->solid()) {
                            loadQuad(quads, manager, primitive::Direction::FRONT, position);
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

        unsigned int vertIdx = 0;

        for (const auto& quad : quads) {
            for (const auto vertex : quad.vertices) {
                vertices.push_back({
                    vertex,
                });
            }

            indices.insert(indices.end(), {
                vertIdx, vertIdx + 1, vertIdx + 2,
                vertIdx, vertIdx + 2, vertIdx + 3,
            });

            vertIdx += 4;
        }

        return createBuffers(vertices, indices);
    }

    void Chunk::draw(const opengl::ShaderProgram& shader) const {
        shader.use();

        // shader.setUniformVec2("texOffset", region.topLeft);
        // shader.setUniformVec2("texScale", {
        //     region.bottomRight.x - region.topLeft.x,
        //     region.bottomRight.y - region.topLeft.y
        // });

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
        const unsigned int idx =  position.z | position.y << CHUNK_SHIFT_OFFSET | position.x << (CHUNK_SHIFT_OFFSET * 2);
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
            getBlock(glm::ivec3(position.x, position.y - 1, position.z)), // down
            getBlock(glm::ivec3(position.x, position.y, position.z - 1)),  // back
        };

        return adjacentBlocks;
    }

    void Chunk::loadQuad(
        std::vector<primitive::Quad> &quads, system::AtlasManager& manager, const primitive::Direction direction, const glm::ivec3 position
    ) const {

        const Block* block = getBlock(position);
        if (!block->solid()) {
            return;
        }

        const auto textureRegion = block->getTextureRegion(manager).value();
        const auto quad = primitive::Quad{ direction, position, textureRegion };
        quads.push_back(quad);
    }

    bool Chunk::createBuffers(const std::vector<primitive::Vertex> &vertices, const std::vector<unsigned int> &indices) {
        glGenVertexArrays(1, &m_vertexArray);
        glGenBuffers(1, &m_vertexBuffer);
        glGenBuffers(1, &m_indexBuffer);

        glBindVertexArray(m_vertexArray);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(primitive::Vertex), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindVertexArray(0);

        m_vertexCount = vertices.size();
        m_indexCount = indices.size();

        return m_indexCount != 0;
    }
}
