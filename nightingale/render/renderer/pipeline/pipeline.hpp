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
            VkDescriptorSetLayout dSet;
            VkPushConstantRange pcRange;
            VkDevice device;
            void createDescriptorSetLayout(VkDevice device);
            void createPushConstantRange();

        public:
            VkPipelineLayout layout;
            PipelineLayout(){};
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

    std::vector<VkDescriptorSet> createDescriptorsets(int maxFrames, VkDevice device, VkDescriptorPool dPool, VkDescriptorSetLayout dLayout, Texture *t);
    VkDescriptorPool createDescriptorPool(VkDevice device, int maxFrames);
    VkDescriptorSetLayout createDescriptorLayout(VkDevice device);
}