//
// Created by brosobad on 23/07/2025.
//

#include "ObjLoader.h"
#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "ImageLoader.h"
#include "Logger.h"

WrapperModel ObjLoader::LoadModel(const std::string& objPath) {
    std::ifstream file(objPath);
    if (!file) {
        CORE_LOG_ERROR("Cannot open OBJ file: {}", objPath);
    }
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec3> temp_normals;
    std::vector<glm::vec2> temp_texCoords;



    std::vector<Mesh> meshes;
    std::unordered_map<std::string, uint32_t> materialsByName;
    std::vector<Material> materials;
    std::vector<Texture> textures;



    std::string parentPath = std::filesystem::path(objPath).parent_path();

    std::string mtlPath = (std::filesystem::path(objPath).parent_path() /
                       (std::filesystem::path(objPath).stem().string() + ".mtl")).string();

    ParseMtlFile(mtlPath, materialsByName, materials, textures);

    // Current state
    std::vector<Vertex> currentVertices;
    std::unordered_map<std::string, uint32_t> currentVertexIndexes;
    auto currentMesh = Mesh();
    uint32_t currentSubMeshOffset = 0;
    std::string currentMaterial;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;

        iss >> prefix;

        if (prefix == "v") {
            glm::vec3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            temp_positions.push_back(pos);
        }

        else if (prefix == "vn") {
            glm::vec3 norm;
            iss >> norm.x >> norm.y >> norm.z;
            temp_normals.push_back(norm);
        }

        else if (prefix == "vt") {
            glm::vec2 uv;
            iss >> uv.x >> uv.y;
            temp_texCoords.push_back(uv);
        }

        else if (prefix == "f") {
            std::string vertexStr;
            int posIdx, texIdx = 0, normIdx = 0;
            std::vector<uint32_t> faceIndices;
            while (iss >> vertexStr) {
                if (!currentVertexIndexes.contains(vertexStr)) {
                    sscanf(vertexStr.c_str(), "%d/%d/%d", &posIdx, &texIdx, &normIdx);
                    Vertex v = {};
                    if (posIdx > 0) v.position = temp_positions[posIdx - 1];
                    if (texIdx > 0) v.texCoord = temp_texCoords[texIdx - 1];
                    if (normIdx > 0) v.normal = temp_normals[normIdx - 1];
                    currentVertices.push_back(v);
                    uint32_t currentIndex = currentVertices.size() - 1;
                    currentVertexIndexes[vertexStr] = currentIndex;
                    faceIndices.push_back(currentIndex);
                } else {
                    faceIndices.push_back(currentVertexIndexes[vertexStr]);
                }
            }

            for (size_t i = 1; i + 1 < faceIndices.size(); ++i) {
                if (faceIndices.size() > 3) CORE_LOG_DEBUG(" > 3");
                Vertex& v0 = currentVertices[faceIndices[0]];
                Vertex& v1 = currentVertices[faceIndices[i]];
                Vertex& v2 = currentVertices[faceIndices[i + 1]];

                glm::vec3 edge1 = v1.position - v0.position;
                glm::vec3 edge2 = v2.position - v0.position;

                glm::vec2 deltaUV1 = v1.texCoord - v0.texCoord;
                glm::vec2 deltaUV2 = v2.texCoord - v0.texCoord;

                float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                glm::vec3 tangent, bitangent;

                tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                tangent = glm::normalize(tangent);

                bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                bitangent = glm::normalize(bitangent);


                v0.tangent += tangent;
                v1.tangent += tangent;
                v2.tangent += tangent;

                v0.bitangent += bitangent;
                v1.bitangent += bitangent;
                v2.bitangent += bitangent;

                currentMesh.indices.push_back(faceIndices[0]);
                currentMesh.indices.push_back(faceIndices[i]);
                currentMesh.indices.push_back(faceIndices[i + 1]);
            }
        }

        else if (prefix == "usemtl") {
            if (currentMesh.indices.size() - currentSubMeshOffset > 0) {
                SubMesh sub{};
                sub.materialIndex = materialsByName[currentMaterial];
                sub.indexOffset = currentSubMeshOffset;
                sub.indexCount = currentMesh.indices.size() - currentSubMeshOffset;
                currentSubMeshOffset += sub.indexCount;
                currentMesh.subMeshes.push_back(sub);
                sub = SubMesh{};
            }
            iss >> currentMaterial;
        }

    }


    if (currentMesh.indices.size() - currentSubMeshOffset > 0) {
        SubMesh sub{};
        sub.materialIndex = materialsByName[currentMaterial];
        sub.indexOffset = currentSubMeshOffset;
        sub.indexCount = currentMesh.indices.size() - currentSubMeshOffset;
        currentSubMeshOffset += sub.indexCount;
        currentMesh.subMeshes.push_back(sub);
        currentMesh.vertices = currentVertices;
        meshes.push_back(currentMesh);
    }

    CORE_LOG_DEBUG("indicates {}, vertices {}, currentSubMeshOffset {}", meshes[0].indices.size(), meshes[0].vertices.size(), currentSubMeshOffset);

    for (auto& v : currentVertices) {
        v.tangent = glm::normalize(v.tangent);
        v.bitangent = glm::normalize(v.bitangent);
    }

    Model model;
    model.meshes = std::move(meshes);
    WrapperModel wrapperModel {};
    wrapperModel.parentPath = parentPath;
    wrapperModel.textures = textures;
    wrapperModel.materials = std::move(materials);
    wrapperModel.model = std::move(model);
    return wrapperModel;
}

void ObjLoader::ParseMtlFile(const std::string &mtlPath,
    std::unordered_map<std::string, uint32_t>& materialsByName,
    std::vector<Material>& materials,
    std::vector<Texture>& textures) {
    std::unordered_map<std::string, uint32_t> texturesByName;
    std::ifstream file(mtlPath);
    if (!file) {
        // throw std::runtime_error("Cannot open MTL file: " + mtlPath);
        CORE_LOG_ERROR("Cannot open MTL file: {}", mtlPath);
    }

    Material currentMaterial;
    std::string currentMaterialName;

    std::string line, prefix;
    while (std::getline(file, line)) {
        // CORE_LOG_DEBUG("line: {}", line);
        std::istringstream iss(line);
        prefix = "";
        iss >> prefix;
        std::string parentPath = std::filesystem::path(mtlPath).parent_path();
        if (prefix == "newmtl") {
            if (!currentMaterial.name.empty()) {
                materials.push_back(currentMaterial);
                materialsByName[currentMaterialName] = materials.size() - 1;
            }
            iss >> currentMaterialName;
            currentMaterial = Material();
            currentMaterial.name = currentMaterialName;
        } else if (prefix == "Ka") {
            iss >> currentMaterial.ambientColor.r >> currentMaterial.ambientColor.g >> currentMaterial.ambientColor.b;
        } else if (prefix == "Kd") {
            iss >> currentMaterial.diffuseColor.r >> currentMaterial.diffuseColor.g >> currentMaterial.diffuseColor.b;
        } else if (prefix == "Ks") {
            iss >> currentMaterial.specularColor.r >> currentMaterial.specularColor.g >> currentMaterial.specularColor.b;
        } else if (prefix == "Ke") {
            iss >> currentMaterial.emissiveColor.r >> currentMaterial.emissiveColor.g >> currentMaterial.emissiveColor.b;
        } else if (prefix == "Ns") {
            iss >> currentMaterial.shininess;
        } else if (prefix == "Ni") {
            iss >>  currentMaterial.IOR;
        } else if (prefix == "d") {
            iss >>  currentMaterial.opacity;
        } else if (prefix == "map_Kd") {
            std::string diffuseTextureName;
            iss >> std::ws;
            std::getline(iss, diffuseTextureName);
            CORE_LOG_DEBUG("path {}", diffuseTextureName );
            Texture texture = ImageLoader::LoadTexture(parentPath + "/textures/" + diffuseTextureName);
            texture.type = Diffuse;

            if (texturesByName.contains(diffuseTextureName)) {
                currentMaterial.textureIndexes[Diffuse] = texturesByName[diffuseTextureName];
            } else {
                currentMaterial.textureIndexes[Diffuse] = textures.size();
                texturesByName[diffuseTextureName] = textures.size();
            }

            textures.push_back(texture);
        } else if (prefix == "map_Bump") {
            std::string bumpTextureName;
            iss >> prefix;
            if (prefix == "-bm") {
                float bumpScale;
                iss >> bumpScale;
                iss >> std::ws;
                std::getline(iss, bumpTextureName);
                CORE_LOG_DEBUG("path {}", bumpTextureName);
                Texture texture = ImageLoader::LoadTexture(parentPath + "/textures/" +  bumpTextureName);
                texture.type = Bump;

                if (texturesByName.contains(bumpTextureName)) {
                    currentMaterial.textureIndexes[Bump] = texturesByName[bumpTextureName];
                } else {
                    currentMaterial.textureIndexes[Bump] = textures.size();
                    texturesByName[bumpTextureName] = textures.size();
                }

                textures.push_back(texture);
            }
        }
    }

    if (!currentMaterial.name.empty()) {
        materials.push_back(currentMaterial);
        materialsByName[currentMaterialName] = materials.size() - 1;
    }
}
