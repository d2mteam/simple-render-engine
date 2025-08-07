//
// Created by brosobad on 21/07/2025.
//

#include "../RenderPipeline.h"

#include <glad/glad.h>

RenderPipeline::RenderPipeline() = default;
RenderPipeline::~RenderPipeline() = default;

void RenderPipeline::SetDepthTest(bool enable) {
    if (enable)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void RenderPipeline::BindShader(uint32_t shaderId) {
    m_ShaderId = shaderId;
    glUseProgram(m_ShaderId);
}

void RenderPipeline::BindUniform(const std::string &name, UniformType type, void *value) const {
    if (m_ShaderId == 0) return;

    GLint location = glGetUniformLocation(m_ShaderId, name.c_str());
    if (location < 0) return; // uniform not found

    switch (type) {
        case Bool:
            glUniform1i(location, *static_cast<const int*>(value));
        case Int:
            glUniform1i(location, *static_cast<const int*>(value));
            break;
        case Float:
            glUniform1f(location, *static_cast<const float*>(value));
            break;
        case Vec2:
            glUniform2fv(location, 1, static_cast<const float*>(value));
            break;
        case Vec3:
            glUniform3fv(location, 1, static_cast<const float*>(value));
            break;
        case Mat4:
            glUniformMatrix4fv(location, 1, GL_FALSE, static_cast<const float*>(value));
            break;
        default:;
    };
}

void RenderPipeline::BindTexture(const uint32_t texId, uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot); // Chọn texture unit 0
    glBindTexture(GL_TEXTURE_2D, texId); // Bind texture vào unit
}


void RenderPipeline::BindVertexArray(uint32_t vaoID) const {
    glBindVertexArray(vaoID);
}

