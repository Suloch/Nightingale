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



        public:
            VkPhysicalDevice physical;
            VkDevice device;
            VkQueue graphics;
            VkQueue present;
            VkSurfaceKHR surface;
            VkInstance instance;
            VkCommandPool commandPool;

            std::vector<const char*>validationLayers;
            bool validationEnabled;
            Device(){}
            Device(VkInstance instance, VkSurfaceKHR surface, bool validation, std::vector<const char*>validationLayers);
            ~Device();

            QueueFamilyIndices findQueueFamilyIndices();
            SwapChainSupportDetails querySwapChainSupport();

            Device& operator=(Device other){
                
                return *this;
            }
    };
}