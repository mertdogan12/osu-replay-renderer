#pragma once

#include "GLFW/glfw3.h"

namespace osuRenderer {
    class Gui
    {
    public:
        Gui(GLFWwindow *window);
        ~Gui();
        void draw();
        void createFrame();
    };
}
