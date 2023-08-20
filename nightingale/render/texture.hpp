
#include<vulkan/vulkan.h>
#include "device.hpp"

namespace ne{

    struct Texture{
        VkImage image;
        VkDeviceMemory memory;
        int height;
        int width;
        int channels;
        VkImageView view;
        VkSampler sampler;
    };
    Texture create_texture_image(
        Device device, 
        VkCommandPool commandPool,
        uint32_t height,
        uint32_t width,
        uint32_t channels
    );
    void transitionImageLayout(
        Device device, 
        VkCommandPool commandPool, 
        VkImage image, 
        VkFormat format, 
        VkImageLayout oldLayout, 
        VkImageLayout newLayout
        );
    void copyBufferToImage(
        Device device, 
        VkCommandPool commandPool, 
        VkBuffer buffer, VkImage image, 
        uint32_t width, 
        uint32_t height
    );

    void createImage(
        Device device,
        VkFormat format, 
        VkImageTiling tiling, 
        VkImageUsageFlags usage, 
        VkMemoryPropertyFlags properties, 
        Texture *tex
    );

    void create_texture_image_view(Device device, Texture *tex);
    VkImageView create_image_view(Device device, VkImage image, VkFormat format);
    Texture create_texture(
        Device device, 
        VkCommandPool commandPool,
        uint32_t height,
        uint32_t width,
        uint32_t channels
    );

    void destroy_texture(Device device, Texture texture);
    void create_sampler(Device device, Texture *texture);
}