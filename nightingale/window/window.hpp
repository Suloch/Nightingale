#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include<vector>
#include<optional>

namespace nge{
    std::vector<const char*> getRequiredExtensions(bool enableValidationLayers);


    class Window{

        public:
            static Window& getInstance(){
                static Window w;
                return w;
            }
            VkSwapchainKHR swapchain;
            GLFWwindow *window;
            
            std::vector<VkImage> images;
            VkFormat format;
            VkExtent2D extent;
            std::vector<VkImageView> image_views;

            void init(char *title, int height, int width);
            void destroy(VkDevice device);
            void run();
            VkSurfaceKHR createSurface(VkInstance instance);
            void createImageViews(VkDevice device);
            void createSwapChain(
                VkPhysicalDevice physical_device, 
                VkDevice device, 
                VkSurfaceKHR surface,
                VkSurfaceCapabilitiesKHR capabilities,
                std::vector<VkSurfaceFormatKHR> formats,
                std::vector<VkPresentModeKHR> present_modes,
                std::optional<uint32_t> graphicsFamily,
                std::optional<uint32_t> presentFamily
            );
            Window(){}

        private:
            VkSurfaceFormatKHR surfaceFormat;
            VkPresentModeKHR presentMode;

            int height;
            int width;
            VkImageView createImageView(VkDevice device, VkImage image, VkFormat format);
            void chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
            void choosePresentMode(const std::vector<VkPresentModeKHR>& modes);
            void chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capablities);
    };

}