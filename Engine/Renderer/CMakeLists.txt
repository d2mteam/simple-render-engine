add_library(Renderer STATIC
        Renderer.h
        RenderBackend.h
        GraphicsDevice.h
        SwapChain.h
        RenderCommandQueue.h
        RenderCommand.h
        RenderConfiguration.h
        SpectatorCamera.cpp
        OpenGL/GraphicsDevice.cpp
        OpenGL/RenderPipeline.cpp
        ResourceManager.h
        ResourceManager.cpp
        UBOData.h

)
find_package(Stb REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(Renderer
        PRIVATE
        glfw
        glad::glad
        glm::glm
        Logger
        Memory
)

target_include_directories(Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})