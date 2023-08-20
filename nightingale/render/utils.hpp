
#include<vulkan/vulkan.h>
#include "device.hpp"

namespace ne{
    void create_buffer(
        Device device, 
        VkDeviceSize size, 
        VkBufferUsageFlags usage, 
        VkMemoryPropertyFlags properties, 
        VkBuffer& buffer, 
        VkDeviceMemory& bufferMemory
    );
    uint32_t findMemoryType(VkPhysicalDevice device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void endSingleTimeCommands(VkCommandPool commandPool, VkCommandBuffer commandBuffer, Device device) ;
    VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool, Device device);
}