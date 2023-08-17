#pragma once

#include <vulkan/vulkan.h>
#include<vector>


namespace ne{
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);
    
    void DestroyDebugUtilsMessengerEXT(
        VkInstance instance, 
        VkDebugUtilsMessengerEXT debugMessenger, 
        const VkAllocationCallbacks* pAllocator
    );

    void create_debug_messenger(VkInstance instance, 
        VkDebugUtilsMessengerEXT *debugMessenger
    );

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    bool checkValidationLayerSupport();

}