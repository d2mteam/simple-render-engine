cmake_minimum_required(VERSION 3.31)
project(simple-render-library)

set(CMAKE_CXX_STANDARD 20)

find_package(Stb REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable(simple-render-library main.cpp)

add_subdirectory(Engine)

target_link_libraries(simple-render-library
        PRIVATE
        glfw
        glad::glad
        glm::glm
        assimp::assimp
        nlohmann_json
        spdlog::spdlog_header_only
        Logger
        Memory
        Renderer
        Model-Loader
)