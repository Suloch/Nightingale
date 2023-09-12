#pragma once

#include<vulkan/vulkan.h>
#include "renderer/pipeline/pipeline.hpp"
#include "renderer/device/device.hpp"
#include "renderer/command/command.hpp"
#include "renderer/buffer/buffer.hpp"
#include "../../window/window.hpp"
#include "../../third-party/imgui/interface.hpp"

namespace nge{
    void renderBuffer(Window *window,
    Device *device, 
    Command *command,
    PipelineLayout *pipelineLayout, 
    Pipeline *pipeline, 
    VkRenderPass renderPass,
    std::vector<GameObjectBuffer *> buffers,
    std::map<std::string, VkDescriptorSet> dSets,
    int currentFrame,
    ImDrawData *drawData

    );

    void recordCommandBuffer(
        Device *device, 
        PipelineLayout *pipelineLayout, 
        Pipeline *pipeline, 
        VkCommandBuffer cBuffer,  
        VkRenderPass renderPass,
        std::vector<GameObjectBuffer *> buffers,
        std::map<std::string, VkDescriptorSet> dSets,
        uint32_t imageIndex,
        ImDrawData *drawData
    );

    void reCreateSwapChain(
        Window *window,
        Device *device,
        VkRenderPass renderPass
    );

}