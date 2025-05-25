#pragma once

namespace minecraft::world {

    enum class BlockType {
        AIR,
        TEST,
    };

    class Block {
    public:
        Block()
            : m_type(BlockType::AIR) {}

        explicit Block(const BlockType type)
            : m_type(type) {}

        [[nodiscard]]
        bool solid() const {
            return m_type != BlockType::AIR;
        }

        [[nodiscard]]
        uint8_t getType() const {
            return static_cast<unsigned int>(m_type);
        }

    private:
        BlockType m_type;
    };
}
