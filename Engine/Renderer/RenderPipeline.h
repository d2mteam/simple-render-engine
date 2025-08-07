//
// Created by brosobad on 18/07/2025.
//

#ifndef RENDER_PIPELINE_H
#define RENDER_PIPELINE_H

#include <cstdint>
#include <string>

enum UniformType {
    Bool,
    Float,
    Vec2,
    Vec3,
    Vec4,
    Mat4,
    Int,
    Sampler2D
};

class RenderPipeline {
public:
    RenderPipeline();
    virtual ~RenderPipeline();
    virtual void SetDepthTest(bool enable);
    virtual void BindShader(uint32_t shaderId);
    virtual void BindUniform(const std::string& name, UniformType type, void* value) const;
    virtual void BindTexture(uint32_t texId, uint32_t slot) const;
    virtual void BindVertexArray(uint32_t vaoId) const;
private:
    uint32_t m_ShaderId = 0;
};



#endif //RENDER_PIPELINE_H
