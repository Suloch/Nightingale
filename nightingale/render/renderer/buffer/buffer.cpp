
#include "buffer.hpp"

#include "../utils.hpp"
#include "../../../logger/logger.hpp"

#include<cstring>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<chrono>

nge::GameObjectBuffer::GameObjectBuffer(VkPhysicalDevice pDevice, VkDevice device, VkQueue graphics, VkCommandPool pool, float x, float y,  float sx, float sy){
    this->device = device;
    x = x - sx / 2 ;
    y = y - sy / 2;

    const std::vector<Vertex> vertices = {
        {{x, y}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{-x, y}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{-x, -y}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{x, -y}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };

    createVertexBuffer(pDevice, device, graphics, pool, vertices);
    createIndexBuffer(pDevice, device, graphics, pool, indices);
    createUniformBuffer(pDevice, device);
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
    vkDestroyBuffer(device, indexBuffer, nullptr);
    vkDestroyBuffer(device, vertexBuffer, nullptr);
    vkDestroyBuffer(device, uniformBuffer, nullptr);
    vkFreeMemory(device, indexMemory, nullptr);
    vkFreeMemory(device, vertexMemory, nullptr);
    vkFreeMemory(device, uniformMemory, nullptr);

}

void nge::GameObjectBuffer::createUniformBuffer(VkPhysicalDevice pDevice, VkDevice device){
     VkDeviceSize bufferSize = sizeof(UniformBufferObject);
     createBuffer(pDevice, device, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffer, uniformMemory);
     vkMapMemory(device, uniformMemory, 0, bufferSize, 0, &uniformBufferMapped);
}


void nge::GameObjectBuffer::updateUniformBuffer(VkExtent2D extent, float x, float y, float z){

    UniformBufferObject ubo{};
    // float time = std::chrono::milliseconds(20).count();
    // static auto startTime = std::chrono::high_resolution_clock::now();

    // auto currentTime = std::chrono::high_resolution_clock::now();
    // float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    // ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    ubo.model = glm::mat4(1.0f);
    ubo.view = glm::lookAt(glm::vec3(x, y, z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), extent.width / (float) extent.height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;
    memcpy(uniformBufferMapped, &ubo, sizeof(ubo));

}