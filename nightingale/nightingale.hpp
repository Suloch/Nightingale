#pragma once


#include <cstdlib>
#include <GLFW/glfw3.h>
#include "render/render.hpp"
#include "render/device.hpp"
namespace ne{

    struct Application{
        GLFWwindow *window;
        Renderer renderer;
        VkPipeline pipeline;
    };

    uint32_t create_application(Application *app);
    uint32_t run_application(Application app);
    u_int32_t destroy_application(Application app);
}

