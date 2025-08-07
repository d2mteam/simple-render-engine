#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "GraphicsDevice.h"
#include "Logger.h"
#include "ObjLoader.h"
#include "RenderPipeline.h"
#include "SpectatorCamera.h"
#include "UBOData.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// MaterialUBO ConvertToMaterialUBO(const Material& material) {
//     MaterialUBO ubo{};
//
//     // Colors
//     ubo.baseColor     = material.baseColor;
//     ubo.diffuseColor  = glm::vec4(material.diffuseColor, 1.0f);
//     ubo.specularColor = glm::vec4(material.specularColor, 1.0f);
//     ubo.emissiveColor = glm::vec4(material.emissiveColor, 1.0f);
//     ubo.ambientColor  = glm::vec4(material.ambientColor, 1.0f);
//
//     // Scalars
//     ubo.scalarParams = glm::vec4(
//         material.roughness,
//         material.metallic,
//         material.shininess,
//         material.opacity
//     );
//
//     // Boolean flags (convert to float for std140 compatibility)
//     ubo.alphaParams = glm::vec4(
//         material.alphaCutoff,
//         material.doubleSided ? 1.0f : 0.0f,
//         material.transparent ? 1.0f : 0.0f,
//         material.alphaMasked ? 1.0f : 0.0f
//     );
//
//     return ubo;
// }


GLuint CreateFallbackGrayTexture() {
    // Màu xám 0.5 (128 / 255)
    unsigned char grayPixel = 128;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(
        GL_TEXTURE_2D, 0,
        GL_RED,             // internal format (1 channel)
        1, 1, 0,
        GL_RED,             // upload format
        GL_UNSIGNED_BYTE,
        &grayPixel          // pointer to data
    );

    // Set sampler state
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return textureID;
}

int main() {



    glm::vec3 lightPos;
    glm::vec3 viewPos(0.0f, 0.0f, 3.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor(1.0f, 1.0f, 1.0f);
    //
    Logger::Init();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA

    GLFWwindow* window = glfwCreateWindow(800, 600, "Rotating Cube", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };

    const VertexFormat VertexFloat1 = {1, GL_FLOAT};
    const VertexFormat VertexFloat2 = {2, GL_FLOAT};
    const VertexFormat VertexFloat3 = {3, GL_FLOAT};
    const VertexFormat VertexFloat4 = {4, GL_FLOAT};

    VertexDeclaration position{
        VertexFloat3,
        sizeof(Vertex),
        nullptr,
        0
        };

    VertexDeclaration normal{
        VertexFloat3,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Normal),
        0
    };

    VertexDeclaration texCoord{
        VertexFloat2,
        sizeof(Vertex),
        (void*)offsetof(Vertex, TexCoords),
        0
    };

    VertexDeclaration tangent{
        VertexFloat3,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Tangent),
        0
    };

    VertexDeclaration bitangent{
        VertexFloat3,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Bitangent),
        0
    };

    GLuint defaultNormalMap;
    glGenTextures(1, &defaultNormalMap);
    glBindTexture(GL_TEXTURE_2D, defaultNormalMap);
    unsigned char defaultNormalPixel[3] = { 128, 128, 255 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, defaultNormalPixel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    // dummy bump


    WrapperModel wrapperModel = ObjLoader::LoadModel("/mnt/MyData/simple-render-engine/Obj/mita/mita.obj");


    struct GpuRefSubMesh {
        uint32_t indexOffset;
        uint32_t indexCount;
        uint32_t materialIndex;
    };

    struct GpuRefMesh {
        std::vector<GpuRefSubMesh> subMeshes;
    };

    struct GpuRefMaterial {
        // uint32_t materialId;
        std::unordered_map<TextureType, uint32_t> textureIndexes;
    };

    struct GpuRefWrapperModel {
        uint32_t vao{};
        std::vector<GpuRefMesh> meshes;
        std::vector<GpuRefMaterial> materials;
        std::vector<uint32_t> textureIds;
    } refWrapperModel;

    std::vector<uint32_t> textureIds;

    VertexDeclaration vertexDeclaration[5] = {position, normal, texCoord, tangent, bitangent};
    GraphicsDevice device;
    RenderPipeline pipeline = RenderPipeline();


    Shader shader = device.CreateShader({"","../Shader-GLSL/test.vert","../Shader-GLSL/phong.frag",""});

    const VertexBuffer vbo = device.CreateVertexBuffer(wrapperModel.model.meshes[0].vertices.size() * sizeof(Vertex), wrapperModel.model.meshes[0].vertices.data());

    const IndexBuffer ibo = device.CreateIndexBuffer(wrapperModel.model.meshes[0].indices.size() * sizeof(uint32_t), wrapperModel.model.meshes[0].indices.data());

    vertexDeclaration[0].buffer = vbo;
    vertexDeclaration[1].buffer = vbo;
    vertexDeclaration[2].buffer = vbo;
    vertexDeclaration[3].buffer = vbo;
    vertexDeclaration[4].buffer = vbo;

    const VertexArray vao = device.CreateVertexArray(vertexDeclaration,5, ibo);

    for (auto const& texture : wrapperModel.textures) {
        uint32_t format = GL_RGBA;
        uint32_t internalFormat = GL_RGBA8;
        switch (texture.channels) {
            case 3:
                format = GL_RGB;
                // internalFormat = (texture.type == Albedo || texture.type == Diffuse) ? GL_SRGB8 : GL_RGB8;
                internalFormat =  GL_RGB8;
                break;
            case 4:
                format = GL_RGBA;
                // internalFormat = (texture.type == Albedo || texture.type == Diffuse) ? GL_SRGB8_ALPHA8 : GL_RGBA8;
                internalFormat =  GL_RGBA8;
                break;
            default:
                break;
        }
        Texture2D texture2d = device.CreateTexture2D(internalFormat, format, texture.width, texture.height, GL_UNSIGNED_BYTE, texture.data);
        refWrapperModel.textureIds.push_back(texture2d.id);
    }
    GpuRefMesh ref_mesh;
    refWrapperModel.meshes.push_back(ref_mesh);

    for (auto const& submesh : wrapperModel.model.meshes[0].subMeshes) {
        GpuRefSubMesh sub_mesh{};
        sub_mesh.indexOffset = submesh.indexOffset;
        sub_mesh.indexCount = submesh.indexCount;
        sub_mesh.materialIndex = submesh.materialIndex;
        refWrapperModel.meshes[0].subMeshes.push_back(sub_mesh);
    }



    for (auto const& mat : wrapperModel.materials) {
        GpuRefMaterial material{};
        material.textureIndexes = mat.textureIndexes;
        //
        // MaterialUBO materialData = ConvertToMaterialUBO(mat);
        //
        // GLuint materialUBO;
        // glGenBuffers(1, &materialUBO);
        // glBindBuffer(GL_UNIFORM_BUFFER, materialUBO);
        // glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialUBO), &materialData, GL_STATIC_DRAW);
        // glBindBuffer(GL_UNIFORM_BUFFER, 0);
        //
        // material.materialId = materialUBO;
        refWrapperModel.materials.push_back(material);
    }

    // glBindBuffer(GL_UNIFORM_BUFFER, materialUBO);
    // glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(MaterialUBO), &materialData);
    // glBindBufferBase(GL_UNIFORM_BUFFER, 1, materialUBO);
    // glBindBuffer(GL_UNIFORM_BUFFER, 0);
    refWrapperModel.vao = vao.id;

    glEnable(GL_DEPTH_TEST);

    // glEnable(GL_CULL_FACE);              // Bật culling
    // glCullFace(GL_BACK);                 // Bỏ qua mặt sau (default)
    // glFrontFace(GL_CCW);                 // Đặt mặt trước là CCW (default)
    // glDisable(GL_CULL_FACE);
    SpectatorCamera camera(window);



    glEnable(GL_MULTISAMPLE);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto model = glm::mat4(1.0f);
        model = glm::scale(model,{0.1f,0.1f,0.1f});

        camera.update(0.02f);
        glm::mat4 view = camera.getViewMatrix();
        // glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 6.0f),
        //                             glm::vec3(0.0f, 0.0f, 0.0f),
        //                             glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = camera.getProjectionMatrix();
        // glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        lightPos = camera.getPosition();
        pipeline.BindShader(shader.id);
        pipeline.BindUniform("view", Mat4, glm::value_ptr(view));
        pipeline.BindUniform("projection", Mat4, glm::value_ptr(projection));
        pipeline.BindUniform("model", Mat4, glm::value_ptr(model));
        pipeline.BindUniform("lightPos", Vec3, glm::value_ptr(lightPos));
        pipeline.BindUniform("lightColor", Vec3, glm::value_ptr(lightColor));
        pipeline.BindUniform("viewPos", Vec3, glm::value_ptr(viewPos));
        pipeline.BindUniform("objectColor", Vec3, glm::value_ptr(objectColor));
        pipeline.BindVertexArray(refWrapperModel.vao);


        for (auto const& submesh : refWrapperModel.meshes[0].subMeshes) {
            GpuRefMaterial ref_material = refWrapperModel.materials[submesh.materialIndex];
            uint32_t diffuseTexture = refWrapperModel.textureIds[ref_material.textureIndexes[Diffuse]];
            uint32_t bumpTexture = defaultNormalMap;
            if (ref_material.textureIndexes.contains(Bump)) {
                bumpTexture = refWrapperModel.textureIds[ref_material.textureIndexes[Bump]];
            }
            pipeline.BindTexture(diffuseTexture, 0);
            pipeline.BindTexture(bumpTexture, 1);

            GLint location = glGetUniformLocation(shader.id, "u_Diffuse");
            glUniform1i(location, 0);
            GLint location1 = glGetUniformLocation(shader.id, "u_Normal");
            glUniform1i(location1, 1);
            // glBindBufferBase(GL_UNIFORM_BUFFER, 1, ref_material.materialId);
            glDrawElements(GL_TRIANGLES, submesh.indexCount, GL_UNSIGNED_INT, (void*)(submesh.indexOffset * sizeof(uint32_t)));
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
