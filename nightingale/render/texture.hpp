#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include<vulkan/vulkan.h>


namespace ne{

    struct Texture{
        VkImage image;
        VkDeviceMemory memory;
        int height;
        int width;
        int channels;
    };
    void create_texture_image();
    void transitionImageLayout(
        Device device, 
        VkCommandPool commandPool, 
        VkCommandBuffer commandBuffer, 
        VkImage image, 
        VkFormat format, 
        VkImageLayout oldLayout, 
        VkImageLayout newLayout
        );
    void copyBufferToImage(
        Device device, 
        VkCommandPool commandPool, 
        VkCommandBuffer commandBuffer, 
        VkBuffer buffer, VkImage image, 
        uint32_t width, 
        uint32_t height
    );

    void createImage(VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, Texture *tex);

}