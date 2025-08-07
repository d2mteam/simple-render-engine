//
// Created by brosobad on 30/07/2025.
//

#ifndef UBO_H
#define UBO_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


struct alignas(16) LightUBO {};

struct alignas(16) MaterialUBO {
    glm::vec4 baseColor;       // 16 bytes
    glm::vec4 diffuseColor;    // 16 bytes (vec3 + padding)
    glm::vec4 specularColor;   // 16 bytes (vec3 + padding)
    glm::vec4 emissiveColor;   // 16 bytes (vec3 + padding)
    glm::vec4 ambientColor;    // 16 bytes (vec3 + padding)
    glm::vec4 scalarParams;    // 16 bytes: roughness, metallic, shininess, opacity
    glm::vec4 alphaParams;     // 16 bytes: alphaCutoff, doubleSided, transparent, alphaMasked
};


struct alignas(16) ModelMaterial {
    glm::vec4 baseColor;
    glm::vec4 diffuseColor;
    glm::vec4 specularColor;
    glm::vec4 emissiveColor;
    glm::vec4 ambientColor;

    float metallic;
    float roughness;
    float ior;
    float alpha;
};
#endif //UBO_H
