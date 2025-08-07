//
// Created by brosobad on 17/07/2025.
//

#ifndef RENDER_BACKEND_H
#define RENDER_BACKEND_H

#include "GraphicsDevice.h"
#include "SwapChain.h"


class RenderBackend {
public:
    virtual void Init() = 0;
    virtual void Shutdown() = 0;
    virtual GraphicsDevice * GetGraphicsDevice() = 0;
    virtual class RenderPipeline* CreateRenderPipeline(const ShaderConfiguration& config) = 0;
    virtual SwapChain * GetSwapChain() = 0;
    virtual class RenderCommandQueue* GetCommandQueue() = 0;
    virtual ~RenderBackend() = default;
};



#endif //RENDER_BACKEND_H
