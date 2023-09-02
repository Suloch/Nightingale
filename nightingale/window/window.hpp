#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include<vector>
#include<optional>

namespace nge{
    std::vector<const char*> getRequiredExtensions(bool enableValidationLayers);


    class Window{

        public:
            GLFWwindow *window;
            
            std::vector<VkImage> images;
            VkFormat format;
            VkExtent2D extent;

            VkSurfaceKHR createSurface(VkInstance instance);
            std::vector<VkImageView> createImageViews(VkDevice device);
            std::vector<VkFramebuffer> createFrameBuffers(VkDevice device, std::vector<VkImageView> imageViews, VkRenderPass renderpass);
            
            Window(const char *title, int height, int width);
            ~Window();

            VkSwapchainKHR createSwapChain(
                VkPhysicalDevice physical_device, 
                VkDevice device, 
                VkSurfaceKHR surface,
                VkSurfaceCapabilitiesKHR capabilities,
                std::vector<VkSurfaceFormatKHR> formats,
                std::vector<VkPresentModeKHR> present_modes,
                std::optional<uint32_t> graphicsFamily,
                std::optional<uint32_t> presentFamily
            );
            VkSwapchainKHR reCreateSwapChain(
                VkPhysicalDevice physical_device, 
                VkDevice device, 
                VkSurfaceKHR surface,
                VkSurfaceCapabilitiesKHR capabilities,
                std::vector<VkSurfaceFormatKHR> formats,
                std::vector<VkPresentModeKHR> present_modes,
                std::optional<uint32_t> graphicsFamily,
                std::optional<uint32_t> presentFamily
            );


        private:
            VkSurfaceFormatKHR surfaceFormat;
            VkPresentModeKHR presentMode;
            VkDevice device;

            int height;
            int width;
            VkImageView createImageView(VkDevice device, VkImage image, VkFormat format);
            void chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
            void choosePresentMode(const std::vector<VkPresentModeKHR>& modes);
            void chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capablities);
    };

}