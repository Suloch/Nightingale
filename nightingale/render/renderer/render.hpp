#pragma once

#include<vulkan/vulkan.h>
#include "renderer/pipeline/pipeline.hpp"
#include "renderer/device/device.hpp"
#include "renderer/command/command.hpp"
#include "../../window/window.hpp"
#include "../../third-party/imgui/interface.hpp"
#include "../gameobject/gameobject.hpp"

namespace nge{
    struct UpdateTexture
    {
        std::string texture;
        GameObject *object;
        int no;
    };

    
    void renderBuffer(Window *window,
    Device *device, 
    Command *command,
    PipelineLayout *pipelineLayout, 
    Pipeline *pipeline, 
    VkRenderPass renderPass,
    std::map<std::string, GameObject *> gameObjects,
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
        std::map<std::string, GameObject *> gameObjects,
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