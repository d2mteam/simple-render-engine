//
// Created by brosobad on 23/07/2025.
//

#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <string>
#include <unordered_map>
#include "RenderModel.h"

class ObjLoader {
public:
    static WrapperModel LoadModel(const std::string& objPath);
private:
    static void ParseMtlFile(const std::string &mtlPath,
        std::unordered_map<std::string, uint32_t> &materialsByName,
        std::vector<Material>& materials,
        std::vector<Texture>& textures);
};

#endif //OBJLOADER_H


// std::unordered_map<std::string, uint32_t>& texturesByName,