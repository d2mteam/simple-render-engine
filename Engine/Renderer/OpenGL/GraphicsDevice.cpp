//
// Created by brosobad on 19/07/2025.
//

#include "../GraphicsDevice.h"
#include <glad/glad.h>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include "Logger.h"


std::string LoadShaderSource(const std::string& path);

uint32_t CompileShader(uint32_t type, const std::string& source);

GraphicsDevice::GraphicsDevice() = default;


GraphicsDevice::~GraphicsDevice() = default;

VertexBuffer GraphicsDevice::CreateVertexBuffer(uint32_t size, const void *data) {
    GLuint vbo;
    glGenBuffers(1, &vbo); 
    glBindBuffer(GL_ARRAY_BUFFER, vbo); 
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); 
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    return {vbo};
}

IndexBuffer GraphicsDevice::CreateIndexBuffer(uint32_t size, const void *data) {
    GLuint ibo;
    glGenBuffers(1, &ibo); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
    return {ibo};
}

VertexArray GraphicsDevice::CreateVertexArray(const VertexDeclaration *vertexDeclarations, uint32_t size, IndexBuffer indexBuffer) {
    GLuint vao;
    glGenVertexArrays(1, &vao); 
    glBindVertexArray(vao); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.id); 

    for (int i = 0; i < size; i++) {
        auto [size, type] = vertexDeclarations[i].format;
        uint32_t stride = vertexDeclarations[i].stride;
        const void * offset = vertexDeclarations[i].offset;
        VertexBuffer vertexBuffer = vertexDeclarations[i].buffer;
        if (vertexBuffer.id != 0) {
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.id);
            glVertexAttribPointer(i, size, type, GL_FALSE, stride, offset);
            glEnableVertexAttribArray(i);
        } else {
            glDisableVertexAttribArray(i);
        }
    }

    glBindVertexArray(0);
    return {vao};
}


Shader GraphicsDevice::CreateShader(const ShaderConfiguration &configuration) {
    std::string vertexCode = LoadShaderSource(configuration.vertexPath);
    std::string fragmentCode = LoadShaderSource(configuration.fragmentPath);

    uint32_t vertex = CompileShader(GL_VERTEX_SHADER, vertexCode);
    uint32_t fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentCode);

    uint32_t shaderID = glCreateProgram();
    glAttachShader(shaderID, vertex);
    glAttachShader(shaderID, fragment);
    glLinkProgram(shaderID);

    int success;
    glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderID, 512, nullptr, infoLog);
        std::cerr << "Shader-GLSL linking error:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return {shaderID};
}

Texture2D GraphicsDevice::CreateTexture2D(
        uint32_t internalFormat,
        uint32_t format,
        uint32_t width,
        uint32_t height,
        uint32_t type,
        const void *pixels,
        bool generatedMipmap)
{
    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);


    return {texId};
}

Sampler GraphicsDevice::CreateSampler(const SamplerConfiguration& config) {
    GLuint samplerId;
    glGenSamplers(1, &samplerId);
    for (const auto& [param, value] : config) {
        if (const auto p = std::get_if<int>(&value)) {
            glSamplerParameteri(samplerId, toGLenum(param), *p);
        }
        if (const auto p = std::get_if<float>(&value)) {
            glSamplerParameterf(samplerId, toGLenum(param), *p);
        }
        if (const auto p = std::get_if<bool>(&value)) {
            glSamplerParameteri(samplerId, toGLenum(param), *p ? GL_TRUE : GL_FALSE);
        }
        if (const auto p = std::get_if<std::array<float,4>>(&value)) {
            glSamplerParameterfv(samplerId, toGLenum(param), p->data());
        }
    }
    return {samplerId};
}

std::string LoadShaderSource(const std::string& path) {
    std::ifstream file(path);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}


uint32_t CompileShader(uint32_t type, const std::string& source) {
    uint32_t shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[1024];  // Kích thước lớn hơn để chứa thông tin đầy đủ
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);

        std::string shaderType = (type == GL_VERTEX_SHADER) ? "VERTEX" :
                                 (type == GL_FRAGMENT_SHADER) ? "FRAGMENT" :
                                 "UNKNOWN";

        LOG_ERROR("Shader-GLSL compile error ( {} ):\n{}", shaderType, infoLog);

        glDeleteShader(shader); // Xóa shader hỏng
        return 0;
    }

    return shader;
}
