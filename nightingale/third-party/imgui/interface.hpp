
#pragma once
#include <vulkan/vulkan.h>
#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"

namespace nge
{
    class Interface{
        private:
            VkDevice device;
            static ImGui_ImplVulkanH_Window g_MainWindowData;
	        VkDescriptorPool imguiPool;

        public:
            Interface(VkDevice device, GLFWwindow *window, VkPhysicalDevice pDevice, VkInstance instance, VkQueue gQueue, VkRenderPass renderpass);
            ~Interface();
            
    };
} // namespace nge

