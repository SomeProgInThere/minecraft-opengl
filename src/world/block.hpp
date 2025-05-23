#pragma once

#include "../system/atlas_manager.hpp"

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

        // TODO: Only supports blocks with 6 same texture sides
        std::optional<system::AtlasRegion> getTextureRegion(system::AtlasManager& manager) const {
            switch (m_type) {
                case BlockType::TEST:
                    return manager.getRegion("test");
                default:
                    return std::nullopt;
            }
        }

    private:
        BlockType m_type;
    };
}
