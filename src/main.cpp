#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "iostream"
#include "fstream"
#include "unordered_map"
#include "cstring"
#include "string"
#include "chrono"
#include "thread"

#include "renderer/Renderer.h"
#include "renderer/Shader.h"
#include "renderer/VertexObject.h"
#include "renderer/Encoder.h"

int main() 
{
    /* GLFW */
    GLFWwindow *window;
    const unsigned int width = 1920, height = 1080;

    // Initialize the library
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(width, height, "Renderer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // GLFW hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    try 
    {
        renderer::Init();
    } catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }

    // Encoder
    // renderer::Encoder encoder("out.mp4", width, height);

    {
        float x = 0.0f, scale = 0.0f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer::Renderer::clear();

            /* Parses the map into vertecies and indicies */
            renderer::SizeStruct sizes = renderer::Renderer::calcCount();

            renderer::Vertex vertecies[sizes.Verticies];
            unsigned int indicies[sizes.Indices];
            renderer::Renderer::parseObjects(vertecies, indicies);

            renderer::Draw(1920.0f, 1080.0f, sizes, vertecies, indicies);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();

            /* Pixels to video */
            // int pixelsSize = width * height * 3; 
            // GLbyte *pixels = new GLbyte[pixelsSize];
            // GLCALL(glReadPixels(0, 0, (float) width, (float) height, GL_RGB, GL_BYTE, pixels));

            // encoder.Write(pixels, pixelsSize);

            // free(pixels);
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
} 
