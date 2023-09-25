#pragma once

#include<vulkan/vulkan.h>
#include "renderer/pipeline/pipeline.hpp"
#include "renderer/device/device.hpp"
#include "renderer/command/command.hpp"
#include "renderer/buffer/buffer.hpp"
#include "../../window/window.hpp"
#include "../../third-party/imgui/interface.hpp"

namespace nge{
    struct UpdateTexture
    {
        std::string texture;
        std::string object;
        GameObject *buffer;
        int no;
    };

    
    void renderBuffer(Window *window,
    Device *device, 
    Command *command,
    PipelineLayout *pipelineLayout, 
    Pipeline *pipeline, 
    VkRenderPass renderPass,
    std::vector<GameObjectBuffer *> buffers,
    std::map<std::string, std::vector<VkDescriptorSet>> dSets,
    int currentFrame,
    ImDrawData *drawData,
    std::map<std::string, Texture *> textures,
     std::vector<UpdateTexture> *uts

    );

    void recordCommandBuffer(
        Device *device, 
        PipelineLayout *pipelineLayout, 
        Pipeline *pipeline, 
        VkCommandBuffer cBuffer,  
        VkRenderPass renderPass,
        std::vector<GameObjectBuffer *> buffers,
        std::map<std::string, std::vector<VkDescriptorSet>> dSets,
        uint32_t imageIndex,
        ImDrawData *drawData,
        int currentFrame
    );

    void reCreateSwapChain(
        Window *window,
        Device *device,
        VkRenderPass renderPass
    );

}