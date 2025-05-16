#include "texture_atlas.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <glad/glad.h>

#include <algorithm>
#include <iostream>

Texture2D::Texture2D()
    : data(nullptr), width(0), height(0), channels(0) {}

Texture2D::~Texture2D() {
    stbi_image_free(data);
}

bool TextureAtlas::addTexture(const std::string& name, const std::string& path) {
    if (regions.contains(name)) {
        return true;
    }

    const std::string texturePath = (assetsPath / path).string();
    const auto texture = std::make_shared<Texture2D>();

    texture->path = texturePath;
    texture->data = stbi_load(
        texturePath.c_str(),
        &texture->width, &texture->height,
        &texture->channels,
        STBI_rgb_alpha
    );

    if (!texture->data) {
        std::cerr << "Failed to load texture for atlas: " << path << std::endl;
        return false;
    }

    pendingTextures.push_back(texture);
    textureCache[name] = texture;
    needsRebuild = true;

    return true;
}

bool TextureAtlas::build() {
    if (!needsRebuild || pendingTextures.empty()) {
        return true;
    }

    std::ranges::sort(
        pendingTextures,
        [](const std::shared_ptr<Texture2D>& a, const std::shared_ptr<Texture2D>& b) {
            return a->height, b->height;
        }
    );

    int area = 0;
    int maxWidth = 0, maxHeight = 0;

    for (const auto& texture : pendingTextures) {
        area += texture->width * texture->height;
        maxWidth = std::max(maxWidth, texture->width);
        maxHeight = std::max(maxHeight, texture->height);
    }

    width = MIN_ATLAS_SIZE;
    height = MIN_ATLAS_SIZE;

    while (width * height < area || width < maxWidth || height < maxHeight) {
        if (width < height) {
            width *= 2;
        } else {
            height *= 2;
        }

        if (width > MAX_ATLAS_SIZE || height > MAX_ATLAS_SIZE) {
            std::cerr << "Atlas size too large. Max size: " << MAX_ATLAS_SIZE << std::endl;
            return false;
        }
    }

    unsigned char* atlasData;
    if (!packTextures(atlasData)) {
        return false;
    }

    if (id != 0) {
        glDeleteTextures(1, &id);
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlasData);
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] atlasData;

    pendingTextures.clear();
    needsRebuild = false;

    std::cout << "Built texture atlas (" << width << "x" << height << ") with "
                  << regions.size() << " textures" << std::endl;

    return true;
}

bool TextureAtlas::save(const std::string& path) const {
    if (id == 0) {
        return false;
    }

    const std::string texturePath = (assetsPath / path).string();
    auto* atlasData = new unsigned char[width * height * 4];

    glBindTexture(GL_TEXTURE_2D, id);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlasData);

    stbi_write_png(texturePath.c_str(), width, height, STBI_rgb_alpha, atlasData, width * 4);

    delete[] atlasData;
    return true;
}

unsigned int TextureAtlas::getID() {
    if (needsRebuild) {
        build();
    }

    return id;
}

const AtlasRegion* TextureAtlas::getRegion(const std::string& name) {
    if (needsRebuild) {
        build();
    }

    const auto regionIter = regions.find(name);
    if (regionIter != regions.end()) {
        return &regionIter->second;
    }

    return nullptr;
}

void TextureAtlas::unloadTexture(const std::string& name) {
    regions.erase(name);
    textureCache.erase(name);
}

void TextureAtlas::unloadAll() {
    if (id != 0) {
        glDeleteTextures(1, &id);
        id = 0;
    }

    regions.clear();
    pendingTextures.clear();
    textureCache.clear();
}

TextureAtlas::TextureAtlas()
    : id(0), width(0), height(0), needsRebuild(false) {

    const std::filesystem::path projectDir = PROJECT_SOURCE_DIR;
    assetsPath = projectDir / "assets";

    stbi_set_flip_vertically_on_load(true);
}

TextureAtlas::~TextureAtlas() {
    unloadAll();
}

bool TextureAtlas::packTextures(unsigned char*& atlasData) {
    atlasData = new unsigned char[width * height * 4];
    int posX = 0, posY = 0;
    int rowHeight = 0;

    regions.clear();

    for (const auto& texture : pendingTextures) {
        std::string textureName;

        for (const auto& [name, cachedTexture] : textureCache) {
            if (cachedTexture == texture) {
                textureName = name;
                break;
            }
        }

        if (textureName.empty()) {
            continue;
        }

        if (posX + texture->width > width) {
            posX = 0;
            posY += rowHeight;
            rowHeight = 0;
        }

        if (posY + texture->height > height) {
            delete[] atlasData;
            std::cerr << "Atlas size too large. Max size: " << MAX_ATLAS_SIZE << std::endl;
            needsRebuild = false;
            return false;
        }

        rowHeight = std::max(rowHeight, texture->height);

        for (int y = 0; y < texture->height; y++) {
            for (int x = 0; x < texture->width; x++) {

                const int srcIdx = (y * texture->width + x) * texture->channels;
                const int dstIdx = ((posY + y) * width + posX + x) * 4;

                for (int ch = 0; ch < std::min(3, texture->channels); ch++) {
                    atlasData[dstIdx + ch] = texture->data[srcIdx + ch];
                }

                atlasData[dstIdx + 3] = texture->channels == 4 ? texture->data[srcIdx + 3] : 255;
            }
        }

        AtlasRegion region{};

        region.topLeft = glm::vec2(
            static_cast<float>(posX) / width,
            static_cast<float>(posY) / height
        );

        region.bottomRight = glm::vec2(
            static_cast<float>(posX + texture->width) / width,
            static_cast<float>(posY + texture->height) / height
        );

        region.width = texture->width;
        region.height = texture->height;

        regions[textureName] = region;
        posX += texture->width;
    }

    return true;
}
