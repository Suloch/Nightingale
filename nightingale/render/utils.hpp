#pragma once

#include<vulkan/vulkan.h>

namespace nge{

    void createBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    uint32_t findMemoryType(VkPhysicalDevice device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
    VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool, VkDevice device);
    void endSingleTimeCommands(VkCommandPool commandPool, VkCommandBuffer commandBuffer, VkDevice device, VkQueue graphics);
    void copyBuffer(VkCommandPool commandPool, VkDevice device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkQueue graphics);
}