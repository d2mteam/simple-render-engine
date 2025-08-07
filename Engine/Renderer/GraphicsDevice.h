//
// Created by brosobad on 18/07/2025.
//

#ifndef GRAPHICS_DEVICE_H
#define GRAPHICS_DEVICE_H


#include <string>
#include <cstdint> //important
#include "RenderConfiguration.h"

struct VertexBuffer {
    uint32_t id;
};

struct IndexBuffer {
    uint32_t id;
};

struct VertexArray {
    uint32_t id;
    // size_t numVertexBuffer;
    // uint32_t* vertexBuffers;
};

struct UniformBuffer {
    uint32_t id;
};

struct Sampler {
    uint32_t id;

};

struct Texture2D {
    uint32_t id;
};

struct CubeTexture {
    uint32_t id;
};

struct VertexFormat {
    uint32_t size : 16;
    uint32_t type : 16;
};

struct VertexDeclaration {
    VertexFormat format;
    uint32_t stride;
    void* offset;
    VertexBuffer buffer;
};

struct Shader {
    uint32_t id;
};

struct ShaderConfiguration {
    std::string shaderName;
    std::string vertexPath;
    std::string fragmentPath;
    std::string geometryPath;
};

struct TextureConfiguration {
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t internalFormat = 0;
    uint32_t format = 0;
    uint32_t type = 0;
    bool generateMipmaps = false;
};


class GraphicsDevice {
public:
    ~GraphicsDevice();
    GraphicsDevice();
    Shader CreateShader(const ShaderConfiguration& config);
    VertexBuffer CreateVertexBuffer(uint32_t size, const void* data);
    IndexBuffer CreateIndexBuffer(uint32_t size, const void* data);
    VertexArray CreateVertexArray(const VertexDeclaration* vertexDeclarations, uint32_t size, IndexBuffer indexBuffer);
    Sampler CreateSampler(const SamplerConfiguration&);
    Texture2D CreateTexture2D(uint32_t internalFormat,
        uint32_t format,
        uint32_t width, uint32_t height,
        uint32_t type, const void *pixels,
        bool generatedMipmap = true);
};

#endif //GRAPHICS_DEVICE_H