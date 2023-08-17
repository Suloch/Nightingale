
#include "render.hpp"
#include "../constants.hpp"
#include<stdexcept>
#include<vector>
#include<iostream>
#include<cstring>
#include<GLFW/glfw3.h>
#include "device.hpp"
#include "validation_layers.hpp"

uint32_t ne::create_renderer(GLFWwindow *window, ne::Renderer *renderer, bool enableValidationLayers){
    ne::create_instance(renderer, enableValidationLayers);
    if(enableValidationLayers){
        ne::create_debug_messenger(renderer -> instance, &renderer -> debugMessenger);
    }
    ne::create_surface(window, renderer->instance, &renderer ->surface);
    ne::create_device(renderer ->instance, renderer ->surface, &renderer -> device, enableValidationLayers, validationLayers);

    return NE_SUCCESS;
}



uint32_t ne::destroy_renderer(ne::Renderer renderer, bool enableValidationLayers){
    ne::destroy_device(renderer.device);
    if(enableValidationLayers){
        DestroyDebugUtilsMessengerEXT(renderer.instance, renderer.debugMessenger, nullptr);
    }
    vkDestroySurfaceKHR(renderer.instance, renderer.surface, nullptr);
    vkDestroyInstance(renderer.instance, nullptr);
    return NE_SUCCESS;
}


std::vector<const char*> getRequiredExtensions(bool enableValidationLayers) {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}


uint32_t ne::create_instance(ne::Renderer *renderer, bool enableValidationLayers){

    if(enableValidationLayers && !ne::checkValidationLayerSupport()){
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = renderer -> application_name;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = renderer -> engine_name;
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;


    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getRequiredExtensions(enableValidationLayers);
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        ne::populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &renderer -> instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
    
    return NE_SUCCESS;
}


uint32_t ne::create_surface(GLFWwindow *window, VkInstance instance, VkSurfaceKHR *surface){
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
    return NE_SUCCESS;

}