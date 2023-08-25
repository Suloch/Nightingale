
#include<vulkan/vulkan.h>
#include "device.hpp"
#include<glm/glm.hpp>
#include<array>

namespace ne{
    struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

            return attributeDescriptions;
        }
    };

    struct Buffer{
        VkBuffer buffer;
        VkDeviceMemory bufferMemory;
    };

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
    Buffer createVertexBuffer(VkCommandPool commandPool, Device device, const std::vector<Vertex> vertices);
    void copyBuffer(VkCommandPool commandPool, Device device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    Buffer createIndexBuffer(VkCommandPool commandPool, Device device, const std::vector<uint16_t> indices);
}