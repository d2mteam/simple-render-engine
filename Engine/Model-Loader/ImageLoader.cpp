//
// Created by brosobad on 24/07/2025.
//

#define STB_IMAGE_IMPLEMENTATION

#include "ImageLoader.h"
#include <stdexcept>
#include <stb_image.h>

#include "Logger.h"

std::unordered_map<std::string, Texture> ImageLoader::loadedTextures;

Texture ImageLoader::LoadTexture(const std::string &path, bool reload) {
    Texture tex;

    int w, h, ch;
    stbi_set_flip_vertically_on_load(true); // nếu cần
    tex.data = stbi_load(path.c_str(), &w, &h, &ch, 0);

    if (!tex.data) {
        throw std::runtime_error("Failed to load image: " + path);
    }
    CORE_LOG_DEBUG("load texture path {},chanel = {}", path, ch);

    tex.width = static_cast<uint32_t>(w);
    tex.height = static_cast<uint32_t>(h);
    tex.channels = static_cast<uint32_t>(ch);
    return tex;
}

void ImageLoader::UnloadAll() {
    // for (auto& [path , tex] : loadedTextures) {
    //     if (tex.data) {
    //         stbi_image_free(tex.data);
    //     }
    // }
    loadedTextures.clear();
}

