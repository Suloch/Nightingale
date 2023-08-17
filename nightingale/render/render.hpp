#pragma once

#include <vulkan/vulkan.h>
#include<vector>
#include "device.hpp"
#include <GLFW/glfw3.h>
#include "swap_chain.hpp"
namespace ne{

    struct Renderer{
        VkInstance instance;
        VkSurfaceKHR surface;
        VkDebugUtilsMessengerEXT debugMessenger;
        Device device;
        SwapChain swap_chain;

        const char *application_name;
        const char *engine_name;
    };
    
    uint32_t destroy_renderer(Renderer renderer, bool enableValidationLayers);
    uint32_t create_renderer(GLFWwindow *window, Renderer *renderer, bool enableValidationLayers);


    uint32_t create_instance(Renderer *renderer, bool enableValidationLayers);


    uint32_t create_surface(GLFWwindow *window, VkInstance instance, VkSurfaceKHR *surface);
    
}

