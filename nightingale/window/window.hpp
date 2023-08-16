#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


namespace ne{
    struct WindowCreateInfo{
        uint32_t height;
        uint32_t width;
        const char *title;
    };

    uint32_t create_window(WindowCreateInfo *windowInfo, GLFWwindow ** window);
    uint32_t destroy_window(GLFWwindow * window);
}