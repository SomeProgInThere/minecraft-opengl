#pragma once

#include "../primitive/texture.hpp"

#include <unordered_map>
#include <memory>
#include <filesystem>
#include <string>

namespace minecraft::system {
    constexpr unsigned int MAX_BLOCK_COUNT = 1;
    constexpr unsigned int MIN_ATLAS_SIZE = 32;
    constexpr unsigned int MAX_ATLAS_SIZE = MIN_ATLAS_SIZE * 4;

    class AtlasManager {
    public:
        AtlasManager();
        ~AtlasManager();

        bool loadAssetDirectory();
        void unloadTexture(std::string_view name);
        void createUniformBuffer();
        void updateUniformBuffer();

        std::optional<primitive::AtlasRegion> getRegion(std::string_view name);

        [[nodiscard]]
        bool save(std::string_view path) const;

        unsigned int getID();
        bool build();
        void unloadAll();

    private:
        struct FaceTexCoords {
            glm::vec2 topLeft;
            glm::vec2 bottomRight;
        };

        struct BlockTextureData {
            FaceTexCoords texCoords[MAX_BLOCK_COUNT * 6];
        };

        bool loadTexture(std::string_view name, std::string_view path);
        bool packTextures(std::vector<unsigned char>& atlasData);

        std::unordered_map<std::string, primitive::AtlasRegion> m_atlasRegions{};
        std::unordered_map<std::string, std::shared_ptr<primitive::Texture>> m_textureCache{};
        std::vector<std::shared_ptr<primitive::Texture>> m_pendingTextures{};

        unsigned int m_id{};
        unsigned int m_uniformBuffer{};

        unsigned int m_atlasWidth{};
        unsigned int m_atlasHeight{};

        bool m_requiresRebuild{};
    };
}
