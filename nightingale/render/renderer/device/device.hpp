#pragma once

#include<vulkan/vulkan.h>
#include<vector>
#include<optional>

namespace nge{
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool is_complete(){
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    class Device{
        private:
            const std::vector<const char*> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };

            void pickPhysicalDevice();
            void createLogicalDevice();
            bool isDeviceSuitable(VkPhysicalDevice device);
            bool checkExtensionSupport(VkPhysicalDevice device);
            void createInstance();
            bool checkValidationLayerSupport();
            VkDebugUtilsMessengerEXT debugMessenger;
            void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
            void createDebugMessenger();
            VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator);
            void DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator);
        public:
            std::vector<const char*> requiredExtensions;
            VkPhysicalDevice physical;
            VkDevice device;
            VkQueue graphics;
            VkQueue present;
            VkSurfaceKHR surface;
            VkInstance instance;
            VkCommandPool commandPool;
            VkSwapchainKHR swapchain;
            VkExtent2D extent;
            std::vector<VkImageView> image_views;
            std::vector<VkFramebuffer> frameBuffers;
            void create();
            bool validationEnabled;
            std::vector<const char*>validationLayers;
            bool framebufferResized = false;
            
            Device(bool validationEnabled, std::vector<const char*>validationLayers, std::vector<const char*> requiredExtensions);
            ~Device();

            QueueFamilyIndices findQueueFamilyIndices(VkPhysicalDevice device);
            SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
            static  VkBool32 VKAPI_CALL debugCallback(
                VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                void* pUserData);
            void cleanSwapChain();
    };
}