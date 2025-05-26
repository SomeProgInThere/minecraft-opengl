#pragma once

#include <stb_image.h>
#include <stb_image_write.h>

#include <glm.hpp>
#include <optional>
#include <vector>
#include <unordered_map>
#include <memory>
#include <filesystem>

namespace minecraft::system {
    const std::filesystem::path SOURCE_DIR = PROJECT_SOURCE_DIR;
    const std::filesystem::path ASSETS_DIR = SOURCE_DIR / "assets";
    const std::filesystem::path SHADERS_DIR = SOURCE_DIR / "shaders";

    struct AtlasRegion {
        int width;
        int height;
        glm::vec2 topLeft;
        glm::vec2 bottomRight;
    };

    constexpr unsigned int MAX_BLOCK_COUNT = 1;
    constexpr unsigned int MIN_ATLAS_SIZE = 32;
    constexpr unsigned int MAX_ATLAS_SIZE = MIN_ATLAS_SIZE * 4;

    class AtlasManager {
    public:
        AtlasManager();
        ~AtlasManager();

        bool loadTexture(std::string_view name, std::string_view path);
        void unloadTexture(std::string_view name);
        void createUniformBuffer();
        void updateUniformBuffer();

        std::optional<AtlasRegion> getRegion(std::string_view name);

        [[nodiscard]]
        bool save(std::string_view path) const;

        unsigned int getID();
        bool build();
        void unloadAll();

    private:
        struct Texture {
            int channels;
            int width;
            int height;
            unsigned char* data;
            std::string path;

            Texture()
                : channels(0), width(0), height(0), data(nullptr) {}

            ~Texture() {
                stbi_image_free(data);
            }
        };

        struct FaceTexCoords {
            glm::vec2 topLeft;
            glm::vec2 bottomRight;
        };

        struct BlockTextureData {
            FaceTexCoords texCoords[MAX_BLOCK_COUNT * 6];
        };

        bool packTextures(std::vector<unsigned char>& atlasData);

        std::unordered_map<std::string, AtlasRegion> m_atlasRegions{};
        std::unordered_map<std::string, std::shared_ptr<Texture>> m_textureCache{};
        std::vector<std::shared_ptr<Texture>> m_pendingTextures{};

        unsigned int m_id{};
        unsigned int m_uniformBuffer{};
        int m_atlasWidth{};
        int m_atlasHeight{};

        bool m_requiresRebuild{};
    };
}
