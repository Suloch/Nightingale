#pragma once

#include<vulkan/vulkan.h>
#include<vector>
#include<optional>
#include <GLFW/glfw3.h>

namespace ne{
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;

        bool is_complete(){
            return graphics_family.has_value();
        }
    };
    struct SwapChain{
        VkSwapchainKHR swapChain;
        std::vector<VkImage> images;
        VkFormat image_format;
        VkExtent2D extent;
        std::vector<VkImageView> image_views;
    };
    SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device, VkSurfaceKHR surface);
    QueueFamilyIndices find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface);
    VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats);
    uint32_t create_swap_chain(GLFWwindow *window, SwapChain *swap_chain, VkPhysicalDevice physical_device, VkDevice device, VkSurfaceKHR surface);
    uint32_t create_image_views(VkDevice device, SwapChain *swap_chain);

}