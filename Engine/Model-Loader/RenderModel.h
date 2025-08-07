//
// Created by brosobad on 02/08/2025.
//

#ifndef RENDER_MODEL_H
#define RENDER_MODEL_H

#include <string>
#include <unordered_map>
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    // glm::ivec4 boneIDs;    // optional - for skinning
    // glm::vec4 weights;     // optional - for skinning
};

struct SubMesh {
    uint32_t indexOffset;
    uint32_t indexCount;
    uint32_t materialIndex;
};

struct Mesh {
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<SubMesh> subMeshes;
};

enum TextureType {Albedo, Diffuse, Normal, Specular, Roughness, Bump, AO};

struct Texture {
    int width;
    int height;
    int channels;
    TextureType type = Diffuse;
    unsigned char* data;
};

struct Material {
    std::string name;

    // Color properties
    glm::vec4 baseColor = glm::vec4(1.0f);  // used in PBR
    glm::vec3 diffuseColor = glm::vec3(1.0f);  // used in Phong
    glm::vec3 specularColor = glm::vec3(0.0f); // Phong model
    glm::vec3 emissiveColor = glm::vec3(0.0f);
    glm::vec3 ambientColor = glm::vec3(0.0f); // optional

    float IOR = 1.5f;
    float shininess = 32.0f; // for Phong
    float opacity = 1.0f;
    float alphaCutoff = 0.5f; // for alpha masked

    // Render flags
    bool doubleSided = false;
    bool transparent = false;
    bool alphaMasked = false;

    std::unordered_map<TextureType, uint32_t> textureIndexes;
};

struct Model {
    std::vector<Mesh> meshes;
    // AABB
    // Skeleton
};

struct WrapperModel {
    std::string parentPath;
    Model model;
    std::vector<Material> materials;
    std::vector<Texture> textures;
};

#endif //RENDER_MODEL_H
