#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui/imgui.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_opengl3.h"

#include "renderer/Renderer.h"
#include "renderer/Shader.h"
#include "renderer/VertexObject.h"
#include "renderer/Encoder.h"

#include "osu-parser/osu!parser.h"

#include "iostream"
#include "fstream"
#include "unordered_map"
#include "string"
#include "chrono"
#include "thread"
#include "math.h"

#include "debugging/Gui.hpp"
#include "commandline/Parse.hpp"

// Calculates the a normalize vector witch points from p1 to p2
glm::vec2 calcDirectionVector(const glm::vec2 &p1, const glm::vec2 &p2)
{
    return p2 - p1;
}

int main(int argc, char* argv[]) 
{
    /* Video outpur file */
    std::ofstream rawVideo;  
    rawVideo.open("out.raw", std::ios::binary | std::ios::out);

    /* Command line options */
    bool gui = osuRenderer::Parse(argv, argv + argc).cmdOptionExists("--gui");

    /* Osu replay parser */
    std::string filePath = "res/replays/Mert - WALKURE - Ikenai Borderline (Speed Up Ver.) [AR 10] (2022-11-04) Osu.osr";
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

    {
        /* Renderer */
        try 
        {
            renderer::Init();
        } catch (std::exception e)
        {
            std::cout << e.what() << std::endl;
            return -1;
        }

        // Textures
        enum TexIds
        {
            CURSOR = 0x00
        };

        const std::string skin = "osu-skin";
        renderer::VertexObject cursor(0, 0, 1, 1, std::string("res/skins/").append(skin).append("/cursor.png"));

        renderer::Renderer::map.insert({TexIds::CURSOR, &cursor});

        /* ImGUI */
        osuRenderer::Gui* imgui;

        if (gui)
            imgui = new osuRenderer::Gui(window);

        // Framerate
        const int FPS = 60;
        
        // The vector witch gets added to the coord each tick.
        glm::vec2 speed(0.0f, 0.0f);

        // remaining time to the next action, time between the two actions
        int ttn = 0;

        // x any y coords of the cursor
        glm::vec2 coords(0, 0);

        // Current action
        int actionCount = 0;

        // When to draw
        const float draw = 1000.0f / FPS;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            if (ttn <= 0)
            {
                actionCount++;
                osuParser::Action action = p.actions[actionCount];
                osuParser::Action nextAction = p.actions[actionCount + 1];

                coords = glm::vec2
                    (
                        width / 512 * action.x,
                        height / 384 * action.y
                     );

                glm::vec2 nextCoords = glm::vec2
                    (
                        width / 512 * nextAction.x,
                        height / 384 * nextAction.y
                     );

                ttn = nextAction.sinceLast;

                // Calculation of the speed
                speed = calcDirectionVector(coords, nextCoords) / float(ttn);
            }

            coords += speed;
            renderer::Renderer::map[TexIds::CURSOR]->ChangeCoords(coords[0], coords[1]);
            
            ttn -= draw;

            // ImGUI
            if (gui)
            {
                imgui->createFrame();  
               
                ImGui::InputInt("Action", &actionCount);

                ImGui::Text("x: %f", coords[0]);
                ImGui::Text("y: %f", coords[1]);

                ImGui::InputInt("Time to next:", &ttn);
            }
            
            // Parses the map into vertecies and indicies
            renderer::SizeStruct sizes = renderer::Renderer::calcCount();

            renderer::Vertex vertecies[sizes.Verticies];
            unsigned int indicies[sizes.Indices];
            renderer::Renderer::parseObjects(vertecies, indicies);

            /* Render here */
            // Clear
            renderer::Renderer::clear();

            // ImGui
            if (gui)
                imgui->draw();

            // Draw
            renderer::Draw(1920.0f, 1080.0f, sizes, vertecies, indicies);

            /* Rendered to Video file */
            int pixelsSize = width * height * 3; 
            GLbyte *pixels = new GLbyte[pixelsSize];
            GLCALL(glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels));

            rawVideo.write((char*) pixels, pixelsSize);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
        
        delete imgui;
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
} 
