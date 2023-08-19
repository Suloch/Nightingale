#pragma once

#include <vulkan/vulkan.h>
#include<vector>
#include "device.hpp"
#include <GLFW/glfw3.h>
#include "swap_chain.hpp"
#define MAX_FRAMES_IN_FLIGHT 2

namespace ne{
    struct Renderer{
        VkInstance instance;
        VkSurfaceKHR surface;
        VkDebugUtilsMessengerEXT debugMessenger;
        Device device;
        SwapChain swap_chain;
        VkRenderPass render_pass;
        VkPipelineLayout pipeline_layout;
        std::vector<VkFramebuffer> frame_buffers;
        VkCommandPool command_pool;
        std::vector<VkCommandBuffer> command_buffers;
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        uint32_t current_frame;
        bool framebufferResized;
        const char *application_name;
        const char *engine_name;
    };
    
    uint32_t destroy_renderer(Renderer renderer, bool enableValidationLayers);
    uint32_t create_renderer(GLFWwindow *window, Renderer *renderer, bool enableValidationLayers);
    

    uint32_t create_instance(Renderer *renderer, bool enableValidationLayers);
    uint32_t create_surface(GLFWwindow *window, VkInstance instance, VkSurfaceKHR *surface);
    uint32_t create_render_pass(Renderer *renderer);
    uint32_t create_frame_buffers(Renderer *renderer);
    uint32_t create_command_pool(Renderer *renderer);
    uint32_t create_command_buffer(Renderer *renderer);
    uint32_t create_sync_objects(Renderer *renderer);
    void recordCommandBuffer(Renderer *renderer, VkCommandBuffer commandBuffer, uint32_t imageIndex, VkPipeline pipeline);
    uint32_t render_frame(GLFWwindow *window, Renderer *renderer, VkPipeline pipeline);
    void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    void recreateSwapChain(GLFWwindow *window, Renderer *renderer);
    void cleanupSwapChain(Renderer renderer);
}

