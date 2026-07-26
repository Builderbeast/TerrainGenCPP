cmake_minimum_required(VERSION 3.10)
project(TerrainGen)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)

add_executable(TerrainGen
    src/main.cpp
    src/NoiseGenerator.cpp
    src/TerrainMap.cpp
    src/BiomeColorBuffer.cpp
    src/Biome.cpp
    src/ImageExporter.cpp
    src/Renderer.cpp
    src/VertexBuffer.cpp
    src/IndexBuffer.cpp
    src/VertexArray.cpp
    src/Shader.cpp
    src/Texture.cpp

    glad/src/glad.c
)

target_include_directories(TerrainGen PRIVATE glad/include)
target_link_libraries(TerrainGen PRIVATE glfw opengl32)