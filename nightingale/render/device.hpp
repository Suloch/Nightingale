#pragma once

#include<vulkan/vulkan.h>
#include<optional>
#include<vector>

namespace ne{
    
    
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    struct Device{
        VkPhysicalDevice physical_device;
        VkDevice device;
        VkQueue graphics_queue;
        VkQueue present_queue;
        VkSurfaceKHR surface;
    };

    uint32_t pick_physical_device(VkPhysicalDevice *device, VkInstance instance);
    uint32_t create_logical_device(
        VkPhysicalDevice physical_device, 
        VkDevice *device, 
        bool enableValidationLayers,
        std::vector<const char*> validationLayers,
        VkQueue *graphics_queue,
        VkQueue *present_queue,
        VkSurfaceKHR surface
    );

    uint32_t create_device(
        VkInstance instance,
        VkSurfaceKHR surface,
        Device *device, 
        bool enableValidationLayers, 
        std::vector<const char*> validationLayers
    );
    uint32_t destroy_device(ne::Device device);
    bool is_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface);
    bool check_device_extension_support(VkPhysicalDevice device);

    uint32_t pick_physical_device(Device *n_device, VkInstance instance, VkSurfaceKHR surface);


}