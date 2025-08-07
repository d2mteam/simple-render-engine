//
// Created by brosobad on 14/07/2025.
//

#ifndef RENDER_COMMAND_QUEUE_H
#define RENDER_COMMAND_QUEUE_H

#include <vector>
#include "MemoryLinearManager.h"
#include "RenderCommand.h"

class RenderCommandQueue {
public:
    RenderCommandQueue(MemoryLinearManager* memoryLinearManager);
    ~RenderCommandQueue();

    void Enqueue(const RenderCommand& callback);
    void ProcessAndRender();
private:
    std::vector<RenderCommand*> mRenderCommandCallbackQueue;
    MemoryLinearManager* mMemoryLinearManager;
};



#endif //RENDER_COMMAND_QUEUE_H
