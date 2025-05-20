#pragma once

#include <glm.hpp>

#include <memory>
#include <string>
#include <filesystem>
#include <unordered_map>

namespace minecraft::systems {

    #define MIN_ATLAS_SIZE 32
    #define MAX_ATLAS_SIZE 128

    struct Texture2D {
        Texture2D();
        ~Texture2D();

        unsigned char* data;
        int width;
        int height;
        int channels;
        std::string path;
    };

    struct AtlasRegion {
        int width;
        int height;
        glm::vec2 topLeft;
        glm::vec2 bottomRight;
    };

    class AtlasManager {
    public:
        AtlasManager();
        ~AtlasManager();

        bool addTexture(std::string_view name, std::string_view path);
        std::optional<AtlasRegion> getRegion(std::string_view name);
        void unloadTexture(std::string_view name);
        [[nodiscard]] bool save(std::string_view path) const;

        unsigned int getID();
        bool build();
        void unloadAll();

    private:
        bool packTextures(std::vector<unsigned char>& atlasData);

        std::unordered_map<std::string, AtlasRegion> m_atlasRegions{};
        std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_textureCache{};
        std::vector<std::shared_ptr<Texture2D>> m_pendingTextures{};

        unsigned int m_id{};
        int m_atlasWidth{};
        int m_atlasHeight{};
        bool m_requiresRebuild{};
    };
}
