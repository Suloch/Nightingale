#pragma once

#include <vulkan/vulkan.h>
#include<vector>


namespace ne{
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    struct Renderer{
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        const char *application_name;
        const char *engine_name;
    };
    
    uint32_t destroy_renderer(Renderer renderer);
    uint32_t create_renderer(Renderer *renderer, bool enableValidationLayers);


    uint32_t create_instance(Renderer *renderer, bool enableValidationLayers);

    void create_debug_messenger(Renderer *renderer, bool enableValidationLayers);

    
}

