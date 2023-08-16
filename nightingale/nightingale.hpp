#pragma once


#include <cstdlib>
#include <GLFW/glfw3.h>

namespace ne{

    struct Application{
        GLFWwindow *window;
    };

    uint32_t create_application(Application *app);
    uint32_t run_application(Application app);
    uint32_t destroy_application(Application app);
}

