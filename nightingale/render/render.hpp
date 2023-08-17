#pragma once

#include <vulkan/vulkan.h>
#include<vector>


namespace ne{

    struct Renderer{
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;

        const char *application_name;
        const char *engine_name;
    };
    
    uint32_t destroy_renderer(Renderer renderer, bool enableValidationLayers);
    uint32_t create_renderer(Renderer *renderer, bool enableValidationLayers);


    uint32_t create_instance(Renderer *renderer, bool enableValidationLayers);


    
}

