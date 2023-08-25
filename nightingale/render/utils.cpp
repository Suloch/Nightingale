#include "utils.hpp"
#include<stdexcept>
#include<cstring>

VkCommandBuffer ne::beginSingleTimeCommands(VkCommandPool commandPool, Device device) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device.device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void ne::endSingleTimeCommands(VkCommandPool commandPool, VkCommandBuffer commandBuffer, Device device) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(device.graphics_queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device.graphics_queue);

    vkFreeCommandBuffers(device.device, commandPool, 1, &commandBuffer);
}

uint32_t ne::findMemoryType(VkPhysicalDevice device, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(device, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}


void ne::create_buffer(Device device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory){
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device.device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device.device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(device.physical_device, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device.device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device.device, buffer, bufferMemory, 0);
}

ne::Buffer ne::createVertexBuffer(VkCommandPool commandPool, Device device, const std::vector<Vertex> vertices) {
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        create_buffer(
            device,
            bufferSize, 
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
            stagingBuffer, 
            stagingBufferMemory
        );
        void* data;
        vkMapMemory(device.device, stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, vertices.data(), (size_t) bufferSize);
        vkUnmapMemory(device.device, stagingBufferMemory);

        
        Buffer buffer;

        create_buffer(
            device, 
            bufferSize, 
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
            buffer.buffer, 
            buffer.bufferMemory
        );

        copyBuffer(commandPool, device, stagingBuffer, buffer.buffer, bufferSize);

        vkDestroyBuffer(device.device, stagingBuffer, nullptr);
        vkFreeMemory(device.device, stagingBufferMemory, nullptr);

        return buffer;
}

void ne::copyBuffer(VkCommandPool commandPool, Device device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands(commandPool, device);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandPool, commandBuffer, device);
}

ne::Buffer ne::createIndexBuffer(VkCommandPool commandPool, Device device, const std::vector<uint16_t> indices) {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    create_buffer(
            device,
            bufferSize, 
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
            stagingBuffer, 
            stagingBufferMemory
        );

    void* data;
    vkMapMemory(device.device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(device.device, stagingBufferMemory);

    Buffer index_buffer;
    create_buffer(
            device, 
            bufferSize, 
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
            index_buffer.buffer, 
            index_buffer.bufferMemory
        );
    copyBuffer(commandPool, device, stagingBuffer, index_buffer.buffer, bufferSize);

    vkDestroyBuffer(device.device, stagingBuffer, nullptr);
    vkFreeMemory(device.device, stagingBufferMemory, nullptr);

    return index_buffer;
}