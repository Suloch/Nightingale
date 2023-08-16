
#include "window.hpp"
#include "../constants.hpp"
#include <cstdlib>

uint32_t ne::create_window(ne::WindowCreateInfo *info, GLFWwindow *window){

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(
        info -> width, 
        info -> height, 
        info -> title, 
        nullptr, 
        nullptr
    );

    return NE_SUCCESS;
}


uint32_t ne::destroy_window(GLFWwindow * window){
    glfwDestroyWindow(window);
    glfwTerminate();
    return NE_SUCCESS;
}