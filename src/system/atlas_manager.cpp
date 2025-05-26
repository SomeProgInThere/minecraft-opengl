#include "atlas_manager.hpp"

#include <glad/glad.h>
#include <iostream>
#include <ranges>

namespace minecraft::system {
    AtlasManager::AtlasManager() {
        stbi_set_flip_vertically_on_load(true);
    }

    AtlasManager::~AtlasManager() {
        unloadAll();
    }

    bool AtlasManager::loadTexture(const std::string_view name, const std::string_view path) {
        if (m_atlasRegions.contains(name.data())) {
            return true;
        }

        const std::string texturePath = (ASSETS_DIR / path).string();
        const auto texture = std::make_shared<Texture>();

        texture->path = texturePath;
        texture->data = stbi_load(
            texturePath.c_str(),
            &texture->width, &texture->height,
            &texture->channels,
            STBI_rgb_alpha
        );

        if (!texture->data) {
            std::cerr << "Failed to load data for texture: " << path << std::endl;
            return false;
        }

        m_pendingTextures.push_back(texture);
        m_textureCache[name.data()] = texture;
        m_requiresRebuild = true;

        return true;
    }

    bool AtlasManager::build() {
        if (!m_requiresRebuild || m_pendingTextures.empty()) {
            return true;
        }

        std::ranges::sort(
            m_pendingTextures,
            [](const std::shared_ptr<Texture>& a, const std::shared_ptr<Texture>& b) {
                return a->height, b->height;
            }
        );

        int area = 0;
        int maxWidth = 0, maxHeight = 0;

        for (const auto& texture : m_pendingTextures) {
            area += texture->width * texture->height;
            maxWidth = std::max(maxWidth, texture->width);
            maxHeight = std::max(maxHeight, texture->height);
        }

        m_atlasWidth = MIN_ATLAS_SIZE;
        m_atlasHeight = MIN_ATLAS_SIZE;

        while (m_atlasWidth * m_atlasHeight < area || m_atlasWidth < maxWidth || m_atlasHeight < maxHeight) {
            if (m_atlasWidth < m_atlasHeight) {
                m_atlasWidth *= 2;
            } else {
                m_atlasHeight *= 2;
            }

            if (m_atlasWidth > MAX_ATLAS_SIZE || m_atlasHeight > MAX_ATLAS_SIZE) {
                std::cerr << "Atlas size too large. Max size: " << MAX_ATLAS_SIZE << std::endl;
                return false;
            }
        }

        std::vector<unsigned char> atlasData;
        if (!packTextures(atlasData)) {
            return false;
        }

        if (m_id != 0) {
            glDeleteTextures(1, &m_id);
        }

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_atlasWidth, m_atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlasData.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        m_pendingTextures.clear();
        m_requiresRebuild = false;

        std::cout << "Built texture atlas (" << m_atlasWidth << "x" << m_atlasHeight << ") with "
                      << m_atlasRegions.size() << " textures" << std::endl;

        return true;
    }

    bool AtlasManager::save(const std::string_view path) const {
        if (m_id == 0) {
            return false;
        }

        const std::string texturePath = (ASSETS_DIR / path).string();
        std::vector<unsigned char> atlasData(m_atlasWidth * m_atlasHeight * 4);

        glBindTexture(GL_TEXTURE_2D, m_id);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlasData.data());

        stbi_flip_vertically_on_write(true);
        stbi_write_png(texturePath.c_str(), m_atlasWidth, m_atlasHeight, STBI_rgb_alpha, atlasData.data(), m_atlasWidth * 4);

        return true;
    }

    unsigned int AtlasManager::getID() {
        if (m_requiresRebuild) {
            build();
        }

        return m_id;
    }

    std::optional<AtlasRegion> AtlasManager::getRegion(const std::string_view name) {
        if (m_requiresRebuild) {
            build();
        }

        if (m_atlasRegions.contains(name.data())) {
            return m_atlasRegions.at(name.data());
        }

        return std::nullopt;
    }

    void AtlasManager::unloadTexture(const std::string_view name) {
        m_atlasRegions.erase(name.data());
        m_textureCache.erase(name.data());
    }

    void AtlasManager::unloadAll() {
        if (m_id != 0) {
            glDeleteTextures(1, &m_id);
            m_id = 0;
        }

        m_atlasRegions.clear();
        m_pendingTextures.clear();
        m_textureCache.clear();
    }

    bool AtlasManager::packTextures(std::vector<unsigned char>& atlasData) {
        atlasData.resize(m_atlasWidth * m_atlasHeight * 4);
        int posX = 0, posY = 0;
        int rowHeight = 0;

        m_atlasRegions.clear();

        for (const auto& texture : m_pendingTextures) {
            std::string textureName;

            for (const auto& [name, cachedTexture] : m_textureCache) {
                if (cachedTexture == texture) {
                    textureName = name;
                    break;
                }
            }

            if (textureName.empty()) {
                continue;
            }

            if (posX + texture->width > m_atlasWidth) {
                posX = 0;
                posY += rowHeight;
                rowHeight = 0;
            }

            if (posY + texture->height > m_atlasHeight) {
                std::cerr << "Atlas size too large. Max size: " << MAX_ATLAS_SIZE << std::endl;
                m_requiresRebuild = false;
                return false;
            }

            rowHeight = std::max(rowHeight, texture->height);

            for (int y = 0; y < texture->height; y++) {
                for (int x = 0; x < texture->width; x++) {

                    const int srcIdx = (y * texture->width + x) * texture->channels;
                    const int dstIdx = ((posY + y) * m_atlasWidth + posX + x) * 4;

                    for (int ch = 0; ch < std::min(3, texture->channels); ch++) {
                        atlasData[dstIdx + ch] = texture->data[srcIdx + ch];
                    }

                    atlasData[dstIdx + 3] = texture->channels == 4
                        ? texture->data[srcIdx + 3]
                        : 255;
                }
            }

            AtlasRegion region{};

            region.topLeft = glm::vec2(
                static_cast<float>(posX) / static_cast<float>(m_atlasWidth),
                static_cast<float>(posY) / static_cast<float>(m_atlasHeight)
            );

            region.bottomRight = glm::vec2(
                static_cast<float>(posX + texture->width) / static_cast<float>(m_atlasWidth),
                static_cast<float>(posY + texture->height) / static_cast<float>(m_atlasHeight)
            );

            region.width = texture->width;
            region.height = texture->height;

            m_atlasRegions[textureName] = region;
            posX += texture->width;
        }

        return true;
    }
}
