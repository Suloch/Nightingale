#pragma once

#include<vulkan/vulkan.h>
#include "device/device.hpp"

namespace nge{

    void createBuffer(Device device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    uint32_t findMemoryType(VkPhysicalDevice device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
    VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool, Device device);
    void endSingleTimeCommands(VkCommandPool commandPool, VkCommandBuffer commandBuffer, Device device);
}