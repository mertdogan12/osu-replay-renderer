#pragma once

namespace osuRenderer {
    class ImGui
    {
    public:
        ImGui(GLFWwindow *window);
        ~ImGui();
        void draw();
        void createFrame();
    };
}
