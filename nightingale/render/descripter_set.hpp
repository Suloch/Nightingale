
#include<vulkan/vulkan.h>
#include "device.hpp"
#include "texture.hpp"

namespace ne{
    VkDescriptorPool createDescriptorPool(int max_frames, Device device);

    std::vector<VkDescriptorSet> createDescriptorSets(
    int max_frames, 
    Device device, 
    VkDescriptorPool descriptor_pool, 
    Texture texture,
    VkDescriptorSetLayout layout
    );
}