#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "renderer/Renderer.h"
#include "renderer/Shader.h"
#include "renderer/VertexObject.h"
#include "renderer/Encoder.h"
#include "osu-parser/osu!parser.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_opengl3.h"

#include "iostream"
#include "fstream"
#include "unordered_map"
#include "cstring"
#include "string"
#include "chrono"
#include "thread"
#include "math.h"

// Calculates the distance between two points (2d)
float calcDistance(const glm::vec2 &p1, const glm::vec2 &p2)
{
    return std::sqrt
        (
         std::pow((p2[0] - p1[0]), 2) + 
         std::pow(p2[1] - p1[1], 2)
         );
}


// Calculates the a normalize vector witch points from p1 to p2
glm::vec2 calcDirectionVector(const glm::vec2 &p1, const glm::vec2 &p2)
{
    return p2 - p1;
}

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

    {
        /* ImGUI */
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 400");

        /* Renderer */
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

        /* Encoder */
        // renderer::Encoder encoder("out.mp4", width, height);
        
        // The vector witch gets added to the coord each tick.
        glm::vec2 speed(0.0f, 0.0f);

        // remaining time to the next action, time between the two actions
        int ttn = 0, ttnBeg = 0;

        // x any y coords of the cursor
        glm::vec2 coords(0, 0);

        // For debugging
        int sleep = 150;
        int actionCount = 0;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Clear */
            renderer::Renderer::clear();

            /* Render here */

            coords += speed;
            renderer::Renderer::map[TexIds::CURSOR]->ChangeCoords(coords[0], coords[1]);

            if (ttn <= 0)
            {
                actionCount++;
                osuParser::Action action = p.actions[actionCount];
                osuParser::Action nextAction = p.actions[actionCount + 1];

                coords = glm::vec2
                    (
                        1920 / 512 * action.x,
                        1080 / 384 * action.y
                     );

                ttn = nextAction.sinceLast;
                ttnBeg = nextAction.sinceLast;

                // Calculation of the speed
                speed = calcDirectionVector(coords, glm::vec2(nextAction.x, nextAction.y)) / float(ttnBeg);
            }
            
            ttn--;

            // ImGUI
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            ImGui::InputInt("Speed", &sleep);
            ImGui::InputInt("Action", &actionCount);

            // ImGui::Text("sineStart: %d", int(action.sinceStart));
            // ImGui::Text("sineLast: %d", int(action.sinceLast));
            ImGui::Text("x: %d", int(coords[0]));
            ImGui::Text("y: %d", int(coords[1]));

            ImGui::InputInt("Time to next:", &ttn);
            ImGui::InputInt("Time to next (Beg):", &ttnBeg);

            // Parses the map into vertecies and indicies
            renderer::SizeStruct sizes = renderer::Renderer::calcCount();

            renderer::Vertex vertecies[sizes.Verticies];
            unsigned int indicies[sizes.Indices];
            renderer::Renderer::parseObjects(vertecies, indicies);

            /* Render */
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
} 
