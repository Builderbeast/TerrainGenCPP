#include "TerrainMap.h"
#include "BiomeColorBuffer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <algorithm>

#include "ImageExporter.h"

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

// Mouse position variables //
double lastMouseX = 0.0, lastMouseY = 0.0;
int windowWidth = 1000, windowHeight = 1000;

// Zoom variables //
float zoom = 1.0f;

// Panning variables //
float panOffsetX = 0.0f, panOffsetY = 0.0f;
bool isDragging = false;
double dragStartX = 0.0, dragStartY = 0.0;
float panStartX = 0.0f, panStartY = 0.0f;

// Callback function for mouse scrolling //
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    float oldZoom = zoom;

    zoom += (float)yOffset * 0.1f * zoom;
    zoom = std::clamp(zoom, 1.0f, 20.0f);

    float mouseNdcX = (float)(lastMouseX / windowWidth) * 2.0f - 1.0f;
    float mouseNdxY = 1.0f - (float)(lastMouseY / windowHeight) * 2.0f;

    panOffsetX += (mouseNdcX * 0.5f) * (1.0f / oldZoom - 1.0f / zoom);
    panOffsetY += (mouseNdxY * 0.5f) * (1.0f / oldZoom - 1.0f / zoom);

    float maxPan = 0.5f * (1.0f - 1.0f / zoom);
    panOffsetX = std::clamp(panOffsetX, -maxPan, maxPan);
    panOffsetY = std::clamp(panOffsetY, -maxPan, maxPan);
}

// Callback function for mouse cursor movement //
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (isDragging)
    {
        double dx = xpos - dragStartX;
        double dy = ypos - dragStartY;

        panOffsetX = panStartX - (float)(dx / windowWidth) * 2.0f / zoom;
        panOffsetY = panStartY + (float)(dy / windowHeight) * 2.0f / zoom;

        float maxPan = 0.5f * (1.0f - 1.0f / zoom);
        panOffsetX = std::clamp(panOffsetX, -maxPan, maxPan);
        panOffsetY = std::clamp(panOffsetY, -maxPan, maxPan);
    }

    lastMouseX = xpos;
    lastMouseY = ypos;
}

// Callback function for mouse button 1 click //
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_1)
    {
        if (action == GLFW_PRESS)
        {
            isDragging = true;
            glfwGetCursorPos(window, &dragStartX, &dragStartY);
            panStartX = panOffsetX;
            panStartY = panOffsetY;
        }
        else if (action == GLFW_RELEASE)
            isDragging = false;
    }
}

int main() {
    // Initializing glfw //
    if (!glfwInit()) {
        std::cout << "Failed to init GLFW\n";
        return -1;
    }

    // Setting up glfw window //
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,  GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "TerrainGen", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create window\n";
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);

    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD\n";
        return -1;
    }
    
    {
        // Vertices for the two triangles rendered in the window //
        float vertices[] = {
            1.0f, 1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 1.0f
        };

        // Indices for vertices to use to render the triangles //
        unsigned int indices[] = {
            0, 1, 2,
            1, 0, 3
        };

        // Enable blending to support transparency //
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // Getting seed from current time //
        int seed = time(0);
        std::cout << std::to_string(seed) << std::endl;

        // Generating terrain map //
        TerrainMap terrain(2000, 2000, seed);

        // Exporting a .ppm of the biome map //
        ImageExporter exporter;
        exporter.ExportBiomeMapToPPM(terrain, "Map_" + std::to_string(seed) + ".ppm");

        // Creating vector with pixel color data //
        std::vector<unsigned char> biomeColorBuffer = GenerateBiomeColorBuffer(terrain);

        // Setting up rendering with OpenGL classes //
        VertexArray va;
        VertexBuffer vb(vertices, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);
       
        Shader shader("../res/shaders/Basic.shader");
        shader.Bind();

        // Creating texture using the biomeColorBuffer //
        Texture texture(biomeColorBuffer, terrain.GetMapWidth(), terrain.GetMapHeight());
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        // Reset states //
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        // Creating renderer //
        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;
        while (!glfwWindowShouldClose(window)) {
            renderer.Clear();

            shader.Bind();
            shader.SetUniform1f("u_Zoom", zoom);
            shader.SetUniform2f("u_PanOffset", panOffsetX, panOffsetY);
            
            renderer.Draw(va, ib, shader);

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}