#pragma once

#include<vulkan/vulkan.h>
#include<glm/glm.hpp>
#include<array>
#include<vector>
#include<string>
#include "../../gameobject/camera.hpp"


namespace nge{
    struct Vertex {
        glm::vec3 pos;
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
    struct TextureBufferObject{
        glm::float32 scale;
        glm::float32 ar;
        glm::float32 x;
        glm::float32 y;
        glm::int32 flipX;
        glm::int32 flipY;
    };
    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
        TextureBufferObject tbo{};  
    };
    class GameObjectBuffer{
        public:
            VkBuffer indexBuffer;
            VkDeviceMemory indexMemory;

            VkBuffer vertexBuffer;
            VkDeviceMemory vertexMemory;
            
            VkBuffer uniformBuffer;
            VkDeviceMemory uniformMemory;
            void *uniformBufferMapped;

            std::string texture;
            

            GameObjectBuffer(VkPhysicalDevice pDevice, VkDevice device, VkQueue graphics, VkCommandPool pool);
            ~GameObjectBuffer();
        
        private:
            VkDevice device;
            void createVertexBuffer(VkPhysicalDevice pDevice, VkDevice device, VkQueue graphics, VkCommandPool pool, const std::vector<Vertex> vertices);
            void createIndexBuffer(VkPhysicalDevice pDevice, VkDevice device, VkQueue graphics, VkCommandPool pool, const std::vector<uint16_t> indices);
            void createUniformBuffer(VkPhysicalDevice pDevice, VkDevice device);
    };
}