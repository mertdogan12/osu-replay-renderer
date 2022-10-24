#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "renderer/Renderer.h"
#include "renderer/Shader.h"
#include "renderer/VertexObject.h"
#include "renderer/Encoder.h"
#include "osu-parser/osu!parser.h"
#include "imgui/imgui.h"

#include "iostream"
#include "fstream"
#include "unordered_map"
#include "cstring"
#include "string"
#include "chrono"
#include "thread"

int main() 
{
    /* Osu replay parser */
    std::string filePath = "res/replays/Mert Dogan - Blue Stahli - Shotgun Senorita (Zardonic Remix) [Insane] (2021-11-11) Osu-1.osr";
    std::ifstream replayFile(filePath, std::ios::binary);

    osuParser::OsrParser p(&replayFile);
    p.Parse();

    /* GLFW */
    GLFWwindow *window;
    const unsigned int width = 1920, height = 1080;

    // Initialize the library
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(width, height, "Renderer", NULL, NULL); if (!window)
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

    /* Textures */
    enum TexIds
    {
        CURSOR = 0x00
    };

    const std::string skin = "osu-skin";
    renderer::VertexObject cursor(0, 0, 1, 1, std::string("res/skins/").append(skin).append("/cursor.png"));

    renderer::Renderer::map.insert({TexIds::CURSOR, &cursor});

    // Encoder
    // renderer::Encoder encoder("out.mp4", width, height);

    {
        int sinceLast = 0;
        int actionCount = 0;
        osuParser::OsTime time = 7500;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Clear */
            renderer::Renderer::clear();

            /* Render here */
            osuParser::Action action = p.actions[actionCount];

            renderer::Renderer::map[TexIds::CURSOR]->ChangeCoords((action.x + sinceLast) * width / 512
                        , (action.y + sinceLast) * height / 384);

            if (time == action.sinceStart)
            {
                std::cout << time << std::endl;

                sinceLast = p.actions[actionCount + 1].sinceLast;
                actionCount++;
            }

            // ImGUI
            int sleep = 0;
            ImGui::InputInt("Speed", &sleep);
            ImGui::Text("Time: %d", time),
            ImGui::Text("sinceStart: %d", action.sinceStart),
            ImGui::Text("sinceLast: %d", sinceLast),

            sinceLast++;
            time++;

            // std::cout << time << " " << i << std::endl;
            // std::cout << time << " " << action.sinceStart << " " << action.x << " " << action.y << std::endl;

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
            
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
} 
