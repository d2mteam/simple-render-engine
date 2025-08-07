//
// Created by brosobad on 14/07/2025.
//

#ifndef RENDERER_H
#define RENDERER_H

struct RendererConfiguration;
class GraphicsDevice;
class RenderBackend;
class SwapChain;
class RenderPipeline;
class DescriptorSet;
class ResourceManager;
class RenderCommand;
class Renderer {
public:

    static void Initialize(const RendererConfiguration&);
    static void BeginScene();
    static void SubmitCmd(const RenderCommand&);
    static void Render();
    static void EndScene();
    static void ShutDown();
private:
        // Context (RenderBackend)
        // RenderBackend (OpenGl, Vulkan, vvv)
        // RenderPipeline
        //      Shader-GLSL
        //      Depth Test
        //      Uniform (camera, position, vv)
        // GraphicsDevice
        //      Vertex Buffer
        //          id
        //          type
        //          get Size
        //          setData
        //      Index Buffer
        //          id
        //          type
        //          get Size
        //          setData
        //      Vertex Array
        //          vertexId;
        //          indexId;
        //          textureId;
        //      Shader-GLSL (config)
        //          id
        //          vert, frag (requirement)
        //          geo (optional)
        //          set uniform()
        //      Texture
        //          id
        //          type
        // Resource Manager (use to allocate render command)
        //      linear allocator
        //      pool allocator
        //      stack allocator
        //      Allocate
        //      Free
        // SwapChain
        // RenderCommandQueue
        // SwapChain (window)
        //      ShowImage()
        // RenderCommandQueue
        //      Executed()
        //      RenderCommand
        //          Type
        //          Data
        // Renderer (context)
        //      Init(Config)
        //      BeginFrame
        //      Render
        //      EndFrame
        //      OnShutdown

        // Create Context
        //      Init(Config)
        //      BeginFrame
                    // {
        //                  RenderPipeline->Bind;
        //                  RendererCommandQueue->Enqueue

                    // }
        //      Render
        //            RendererCommandQueue->Executed
        //            SwapChain->show image
        //      EndFrame
                    // {
                    //  ResourceManager->Free
                    // }
        //      OnShutdown
        // Renderer(Context)



        // Mesh -> covert (Index Buffer, Vertex Buffer, Vertex Array, Texture) -> setData Command
        // -> draw command

    RenderBackend* renderBackend; // config window
    // config backend, window
    // factory for :
    // GraphicsDevice
    //      Vertex Buffer
    //      Index Buffer
    //      Vertex Array
    //      Shader-GLSL

    GraphicsDevice* graphicsDevice;
    // factory for
    // descriptor set,
    // vertex array, blind vertex buffer, index buffer, texture, shader

    // vertex buffer,
            // id
            // type
            // get Size
            // setData
            // load data
            // up data command

    // index buffer,
            // id
            // getSize
            // setData
            // load data
            // up data command

    // vertex array
            // vertex buffer
            // vertex index
            // bind command, bind index, vertex,

    // shader program (config)
            // id
            // vert, frag (requirement)
            // geo (optional)
            // set uniform
            // vvv

    // texture
        // id
        // type
        // path

    ResourceManager* resourceManager;
    // manager memory resource;
    // allocator like stack, linear, pool
    // allocate, free, vv
    RenderPipeline* renderPipeline; //blind shader, depth test, config, vvv
    // blind shader(shaderID);
    // config depth test, vv
    SwapChain* swapChain;
    // get index image, show image, double/triple buffering

    RenderCommandQueue* renderCommandQueue; // execute render command
    // execute logic
    // sort command order
    // executed command
    // enqueue

    // RenderCommand
    // command type :
    // draw
    // load resource to gpu, blind texture, blind buffer, blind descriptorSet, vv
    // clear buffer
    // lamda (test)
    // frameId

    // model -> mesh
    // (Mesh -> DescriptorSet, VertexArray (VAO), Vertex Buffer, Index Buffer, Texture, DrawInfo, Material ...) //data-only
    DescriptorSet* descriptorSet; // bind global uniform like view, position, light, texture vvv
    // mesh: how to store vertex, index, texture, transform -> covert to render command


    // Model Model-Loader (outer)

        // load data ->get id and free
public:
    ~Renderer() = default;
protected:
    Renderer() = default;
};

#endif //RENDERER_H
