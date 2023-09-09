#pragma  once

#include <vulkan/vulkan.h>
#include "utils.hpp"
namespace nge{
    class Texture{
        private:
            VkImage image;
            VkDeviceMemory memory;

            VkDevice device;
            VkPhysicalDevice pDevice;
            VkCommandPool commandPool;
            VkQueue graphics;
            int height;
            int channels;
            int width;
            const char *name;
            const char* path;
            void createTextureImage();
            void createImage(VkFormat format,  VkImageTiling tiling,  VkImageUsageFlags usage,  VkMemoryPropertyFlags properties);
            void transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
            void copyBufferToImage(VkBuffer buffer);
            void createTextureImageView(VkFormat format);
            void createSampler();
            
        public:
            int flipX = 1;
            int flipY = 1;
            VkImageView view;
            VkSampler sampler;
            Texture(VkCommandPool commandPool, VkQueue graphics, VkPhysicalDevice pDevice, VkDevice device, const char* name, const char* filepath);
            ~Texture();
            float getAspectRatio();
    };
}