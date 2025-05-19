#pragma once

#include <glm.hpp>

#include <memory>
#include <string>
#include <filesystem>
#include <unordered_map>

namespace minecraft::system {

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

        bool addTexture(const std::string& name, const std::string& path);
        const AtlasRegion* getRegion(const std::string& name);
        void unloadTexture(const std::string& name);
        [[nodiscard]] bool save(const std::string& path) const;

        unsigned int getID();
        bool build();
        void unloadAll();

    private:
        bool packTextures(unsigned char*& atlasData);

        std::unordered_map<std::string, AtlasRegion> m_atlasRegions;
        std::vector<std::shared_ptr<Texture2D>> m_pendingTextures;
        std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_textureCache;

        std::filesystem::path m_assetsPath;

        unsigned int m_id{};
        int m_atlasWidth{};
        int m_atlasHeight{};
        bool m_requiresRebuild{};
    };
}
