
#include "command.hpp"
#include<stdexcept>
#include<optional>

nge::Command::Command(VkDevice device,std::optional<uint32_t> graphicsFamily){
    this->device = device;
    createCommandPool(graphicsFamily);
    createCommandBuffer();
}


void nge::Command::createCommandBuffer(){
    buffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) buffers.size();

    if (vkAllocateCommandBuffers(device, &allocInfo, buffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void nge::Command::createCommandPool(std::optional<uint32_t> graphicsFamily){

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = graphicsFamily.value();

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics command pool!");
    }
}


nge::Command::~Command(){
    vkDestroyCommandPool(device, pool, nullptr);
}

