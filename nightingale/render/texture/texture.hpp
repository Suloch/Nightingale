#pragma  once

#include <vulkan/vulkan.h>
#include "utils.hpp"
namespace nge{
    class Texture{
        private:
            VkImage image;
            VkDeviceMemory memory;
            VkImageView view;
            VkSampler sampler;

            Device& device;
            VkCommandPool commandPool;

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
            Texture(Device& device, const char* name, const char* filepath);
            ~Texture();
    };
}