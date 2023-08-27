
#include "descripter_set.hpp"
#include<stdexcept>
#include<array>

VkDescriptorPool ne::createDescriptorPool(int max_frames, Device device) {

    VkDescriptorPool descriptor_pool;
    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    // poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    // poolSizes[0].descriptorCount = static_cast<uint32_t>(max_frames);
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(max_frames);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(max_frames);

    if (vkCreateDescriptorPool(device.device, &poolInfo, nullptr, &descriptor_pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }

    return descriptor_pool;
}


std::vector<VkDescriptorSet> ne::createDescriptorSets(
    int max_frames, 
    Device device, 
    VkDescriptorPool descriptor_pool, 
    Texture texture,
    VkDescriptorSetLayout layout
    ) {
    
    std::vector<VkDescriptorSetLayout> layouts(max_frames, layout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptor_pool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(max_frames);
    allocInfo.pSetLayouts = layouts.data();
    
    std::vector<VkDescriptorSet> descriptor_sets;
    descriptor_sets.resize(max_frames);
    if (vkAllocateDescriptorSets(device.device, &allocInfo, descriptor_sets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < max_frames; i++) {
        // VkDescriptorBufferInfo bufferInfo{};
        // bufferInfo.buffer = uniformBuffers[i];
        // bufferInfo.offset = 0;
        // bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = texture.view;
        imageInfo.sampler = texture.sampler;

        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

        // descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        // descriptorWrites[0].dstSet = descriptor_sets[i];
        // descriptorWrites[0].dstBinding = 0;
        // descriptorWrites[0].dstArrayElement = 0;
        // descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        // descriptorWrites[0].descriptorCount = 1;
        // descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptor_sets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(device.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

    }

    return descriptor_sets;
}

