
#include "buffer.hpp"

#include "../utils.hpp"

#include<cstring>

nge::GameObjectBuffer::GameObjectBuffer(VkPhysicalDevice pDevice, VkDevice device, VkQueue graphics, VkCommandPool pool, float x, float y,  float sx, float sy){
    x = x - sx / 2 ;
    y = y - sy / 2;

    const std::vector<Vertex> vertices = {
        {{-x, -y}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{x, -y}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{x, y}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{-x, y}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };

    createVertexBuffer(pDevice, device, graphics, pool, vertices);
    createIndexBuffer(pDevice, device, graphics, pool, indices);
}

void nge::GameObjectBuffer::createVertexBuffer(VkPhysicalDevice pDevice, VkDevice device, VkQueue graphics, VkCommandPool pool, const std::vector<Vertex> vertices){
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(
            pDevice,
            device,
            bufferSize, 
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
            stagingBuffer, 
            stagingBufferMemory
        );
        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, vertices.data(), (size_t) bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);

        

        createBuffer(
            pDevice,
            device, 
            bufferSize, 
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
            vertexBuffer, 
            vertexMemory
        );

        copyBuffer(pool, device, stagingBuffer, vertexBuffer, bufferSize, graphics);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);

}


void nge::GameObjectBuffer::createIndexBuffer(VkPhysicalDevice pDevice, VkDevice device, VkQueue graphics, VkCommandPool pool, const std::vector<uint16_t> indices){
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(
        pDevice,
        device,
        bufferSize, 
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
        stagingBuffer, 
        stagingBufferMemory
        );

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    createBuffer(
        pDevice,
        device, 
        bufferSize, 
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
        indexBuffer, 
        indexMemory
        );
    copyBuffer(pool, device, stagingBuffer, indexBuffer, bufferSize, graphics);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

nge::GameObjectBuffer::~GameObjectBuffer(){
    
}