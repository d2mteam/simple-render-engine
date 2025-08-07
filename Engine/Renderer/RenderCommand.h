//
// Created by brosobad on 18/07/2025.
//

#ifndef RENDER_COMMAND_H
#define RENDER_COMMAND_H

enum class RenderCommandType {
    Draw,
    Clear,
    SetPipeline,
    SetVertexBuffer,
    SetIndexBuffer,
};

class RenderCommand {
public:
    RenderCommandType type;
    void* data;
};



#endif //RENDER_COMMAND_H
