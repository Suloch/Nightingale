#pragma once
#include<vulkan/vulkan.h>
#include<vector>
#include<string>
#include<fstream>

namespace ne{
    VkShaderModule create_shader(VkDevice device, const std::string& filename);    
}