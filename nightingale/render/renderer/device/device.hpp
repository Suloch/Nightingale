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
            bool isDeviceSuitable();
            bool checkExtensionSupport();
            void createInstance();
            bool checkValidationLayerSupport();
            void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        public:
            VkPhysicalDevice physical;
            VkDevice device;
            VkQueue graphics;
            VkQueue present;
            VkSurfaceKHR surface;
            VkInstance instance;
            VkCommandPool commandPool;

            
            bool validationEnabled;
            std::vector<const char*>validationLayers;

            static Device& getInstance(){
                static Device device;
                return device;
            }
            Device(){};
            ~Device();
            
            void init(bool validationEnabled, std::vector<const char*>validationLayers);
            QueueFamilyIndices findQueueFamilyIndices();
            SwapChainSupportDetails querySwapChainSupport();
            static  VkBool32 VKAPI_CALL debugCallback(
                VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                void* pUserData);
    };
}