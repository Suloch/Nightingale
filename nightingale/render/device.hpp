#pragma once

#include<vulkan/vulkan.h>
#include<optional>
#include<vector>

namespace ne{
    
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

        bool is_complete(){
            return graphics_family.has_value();
        }
    };
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
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

    SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device, VkSurfaceKHR surface);
    QueueFamilyIndices find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface);
    uint32_t pick_physical_device(Device *n_device, VkInstance instance, VkSurfaceKHR surface);


}