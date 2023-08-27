#pragma once 

#include<vulkan/vulkan.h>
#include<array>
#include <glm/glm.hpp>
#include "utils.hpp"
namespace ne{
    

    struct Vertex2{
        glm::vec2 pos;
        glm::float32 zoom;
    };
    VkPipeline create_piepline2(VkDevice device, VkRenderPass render_pass);
    VkPipeline create_graphice_pipline(VkDevice device, VkRenderPass render_pass, VkPipelineLayout pipeline_layout);
    void create_graphice_pipline_layout(VkDevice device, VkPipelineLayout *pipeline_layout,  VkDescriptorSetLayout *set_layout);
    VkPushConstantRange create_push_constant();
    void create_graphics_set_layout(VkDevice device, VkDescriptorSetLayout *descriptor_set_layout);
}