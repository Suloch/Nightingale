#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>
#include<string>
#include "texture/texture.hpp"

namespace nge{
    struct Vertex2{
        glm::vec2 pos;
        glm::float32 zoom;
    };
    class PipelineLayout{
        
        private:
            VkPushConstantRange pcRange={};
            VkDevice device;
            void createDescriptorSetLayout();
            void createPushConstantRange();

        public:
            VkDescriptorSetLayout dSet;
            VkDescriptorSetLayout smaplerDescriptorSetLayout;
            VkDescriptorSetLayout uboDescriptorSetLayout;
            VkPipelineLayout layout;
            PipelineLayout(VkDevice device);
            ~PipelineLayout();
    };

    class SyncObjects{
        private:
            VkDevice device;

        public:
            std::vector<VkSemaphore> imageAvailableSemaphores;
            std::vector<VkSemaphore> renderFinishedSemaphores;
            std::vector<VkFence> inFlightFences;

            SyncObjects(int frames, VkDevice device);
            ~SyncObjects();


    };

    class Pipeline{
        public:
            char *name;
            VkDevice device;
            VkPipeline pipeline;
            SyncObjects *syncObjects;

            Pipeline(char *name, VkDevice device, VkPipelineLayout pipelineLayout, VkRenderPass renderPass, char *fragPath, char *vertPath);
            ~Pipeline();
        private:
            VkShaderModule createShader(VkDevice device, const std::string& filename);  
    };

    std::vector<VkDescriptorSet> createDescriptorsets(int maxFrames, VkDevice device, VkDescriptorPool dPool, VkDescriptorSetLayout dLayout);
    VkDescriptorPool createDescriptorPool(VkDevice device, int maxFrames);
    void updateDescriptorSet(VkDevice device, VkDescriptorSet descriptorSet,  Texture *t, VkBuffer);
    VkDescriptorSet createDescriptorset(VkDevice device, VkDescriptorPool dPool, VkDescriptorSetLayout dLayout);
}