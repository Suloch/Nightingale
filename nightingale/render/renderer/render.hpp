#pragma once

#include<vulkan/vulkan.h>
#include "renderer/pipeline/pipeline.hpp"
#include "renderer/device/device.hpp"
#include "renderer/command/command.hpp"
#include "renderer/buffer/buffer.hpp"

namespace nge{
    void renderBuffer(Device *device);
    void recordCommandBuffer(
        Device *device, 
    PipelineLayout *pipelineLayout, 
    Pipeline *pipeline, 
    VkCommandBuffer cBuffer,  
    VkRenderPass renderPass,
    uint32_t imageIndex,
    GameObjectBuffer buffer
    );
}