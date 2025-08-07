//
// Created by brosobad on 24/07/2025.
//

#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <cstdint>
#include <string>
#include <unordered_map>

#include "RenderModel.h"

class ImageLoader {
public:
    static Texture LoadTexture(const std::string& path, bool reload = false);
    static void UnloadAll();
private:
    static std::unordered_map<std::string, Texture> loadedTextures;
};

#endif //IMAGELOADER_H
