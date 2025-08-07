//
// Created by brosobad on 28/07/2025.
//

#ifndef RENDER_CONFIGURATION_H
#define RENDER_CONFIGURATION_H

#include <unordered_map>
#include <array>
#include <variant>
#include <glad/glad.h>

enum class SamplerParam {
    MinFilter,
    MagFilter,
    WrapS,
    WrapT,
    WrapR,
    LodBias,
    MinLod,
    MaxLod,
    CompareMode,
    CompareFunc,
    Anisotropy
};

using SamplerValue = std::variant<int, float, bool, std::array<float,4>>;

using SamplerConfiguration = std::unordered_map<SamplerParam, SamplerValue>;

static const SamplerConfiguration standardSampler = {
    {SamplerParam::MinFilter,    GL_LINEAR_MIPMAP_LINEAR},  // dùng mipmap, lọc mượt
    {SamplerParam::MagFilter,    GL_LINEAR},                // phóng đại mượt

    {SamplerParam::WrapS,        GL_CLAMP_TO_BORDER},
    {SamplerParam::WrapT,        GL_CLAMP_TO_BORDER},


    {SamplerParam::LodBias,      0.0f},
    {SamplerParam::MinLod,       -1000.0f},
    {SamplerParam::MaxLod,       1000.0f},
};

inline GLenum toGLenum(SamplerParam param) {
    switch (param) {
        case SamplerParam::MinFilter: return GL_TEXTURE_MIN_FILTER;
        case SamplerParam::MagFilter: return GL_TEXTURE_MAG_FILTER;
        case SamplerParam::WrapS:     return GL_TEXTURE_WRAP_S;
        case SamplerParam::WrapT:     return GL_TEXTURE_WRAP_T;
        case SamplerParam::WrapR:     return GL_TEXTURE_WRAP_R;
        case SamplerParam::LodBias:   return GL_TEXTURE_LOD_BIAS;
        case SamplerParam::MinLod:    return GL_TEXTURE_MIN_LOD;
        case SamplerParam::MaxLod:    return GL_TEXTURE_MAX_LOD;
        case SamplerParam::CompareMode: return GL_TEXTURE_COMPARE_MODE;
        case SamplerParam::CompareFunc: return GL_TEXTURE_COMPARE_FUNC;
        // case SamplerParam::Anisotropy: return GL_TEXTURE_MAX_ANISOTROPY_EXT;
        default: return 0;
    }
}



#endif //RENDER_CONFIGURATION_H
