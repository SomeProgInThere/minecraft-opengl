#pragma once

#include "vec2.hpp"

#include <memory>
#include <string>
#include <filesystem>
#include <unordered_map>

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

class TextureAtlas {
public:
    TextureAtlas();
    ~TextureAtlas();

    bool addTexture(const std::string& name, const std::string& path);
    const AtlasRegion* getRegion(const std::string& name);
    void unloadTexture(const std::string& name);
    [[nodiscard]] bool save(const std::string& path) const;

    unsigned int getID();
    bool build();
    void unloadAll();

private:
    bool packTextures(unsigned char*& atlasData);

    std::unordered_map<std::string, AtlasRegion> regions;
    std::vector<std::shared_ptr<Texture2D>> pendingTextures;
    std::unordered_map<std::string, std::shared_ptr<Texture2D>> textureCache;

    std::filesystem::path assetsPath;

    unsigned int id;
    int width;
    int height;
    bool needsRebuild;
};