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
            int width;
            int channels;
            const char *name;
            const char* path;

            void createTextureImage();
            void createImage(VkFormat format,  VkImageTiling tiling,  VkImageUsageFlags usage,  VkMemoryPropertyFlags properties);
            void transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
            void copyBufferToImage(VkBuffer buffer);
            void createTextureImageView(VkFormat format);
            void createSampler();
            
        public:
            VkImageView view;
            VkSampler sampler;
            Texture(VkQueue graphics, VkPhysicalDevice pDevice, VkDevice device, const char* name, const char* filepath);
            ~Texture();
    };
}