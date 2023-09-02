#pragma once

#include<vulkan/vulkan.h>
#include<vector>
#include<optional>
namespace nge{
    class Command{

        public:
            Command(VkDevice device,std::optional<uint32_t> graphicsFamily);
            ~Command();
            VkCommandPool pool;
            std::vector<VkCommandBuffer> buffers;

          
        private:
            const int MAX_FRAMES_IN_FLIGHT=2;
            VkDevice device;
            void createCommandPool(std::optional<uint32_t> graphicsFamily);
            void createCommandBuffer();
    };

    
}