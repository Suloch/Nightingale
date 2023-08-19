
#include "render.hpp"
#include "../constants.hpp"
#include<stdexcept>
#include<vector>
#include<iostream>
#include<cstring>
#include<GLFW/glfw3.h>
#include "device.hpp"
#include "validation_layers.hpp"
#include "pipeline.hpp"





uint32_t ne::create_renderer(GLFWwindow *window, ne::Renderer *renderer, bool enableValidationLayers){
    ne::create_instance(renderer, enableValidationLayers);
    if(enableValidationLayers){
        ne::create_debug_messenger(renderer -> instance, &renderer -> debugMessenger);
    }
    ne::create_surface(window, renderer->instance, &renderer ->surface);
    ne::create_device(renderer ->instance, renderer ->surface, &renderer -> device, enableValidationLayers, validationLayers);
    ne::create_swap_chain(window, &renderer -> swap_chain, renderer->device.physical_device, renderer->device.device, renderer->surface);
    ne::create_image_views(renderer->device.device, &renderer->swap_chain);
    ne::create_render_pass(renderer);
    ne::create_frame_buffers(renderer);
    ne::create_graphice_pipline_layout(renderer->device.device, &renderer->pipeline_layout);
    ne::create_command_pool(renderer);
    ne::create_command_buffer(renderer);
    ne::create_sync_objects(renderer);
    renderer -> current_frame = 0;

    
    
    return NE_SUCCESS;
}

void ne::recordCommandBuffer(Renderer *renderer, VkCommandBuffer commandBuffer, uint32_t imageIndex, VkPipeline pipeline){
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderer -> render_pass;
    renderPassInfo.framebuffer = renderer->frame_buffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = renderer->swap_chain.extent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) renderer->swap_chain.extent.width;
    viewport.height = (float) renderer->swap_chain.extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = renderer->swap_chain.extent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    // VkBuffer vertexBuffers[] = {vertexBuffer};
    // VkDeviceSize offsets[] = {0};
    vkCmdDraw(commandBuffer, 3, 1, 0, 0);
    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }

}

uint32_t ne::create_sync_objects(Renderer *renderer){
    renderer -> imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderer ->renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderer ->inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(renderer->device.device, &semaphoreInfo, nullptr, &renderer -> imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(renderer->device.device, &semaphoreInfo, nullptr, &renderer -> renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(renderer->device.device, &fenceInfo, nullptr, &renderer->inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
    return NE_SUCCESS;
}

void ne::framebufferResizeCallback(GLFWwindow* window, int width, int height){
    auto renderer = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
    renderer->framebufferResized = true;
}

void ne::recreateSwapChain(GLFWwindow *window, Renderer *renderer) {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(renderer->device.device);

    cleanupSwapChain(*renderer);
    ne::create_swap_chain(window, &renderer -> swap_chain, renderer->device.physical_device, renderer->device.device, renderer->surface);
    ne::create_image_views(renderer->device.device, &renderer->swap_chain);
    ne::create_frame_buffers(renderer);
}



void ne::cleanupSwapChain(Renderer renderer){
    std::cout<<"Deleting swapchain"<<std::endl;
    for (auto framebuffer : renderer.frame_buffers) {
        vkDestroyFramebuffer(renderer.device.device, framebuffer, nullptr);
    }

    for (auto imageView : renderer.swap_chain.image_views) {
        vkDestroyImageView(renderer.device.device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(renderer.device.device, renderer.swap_chain.swapChain, nullptr);
}

uint32_t ne::render_frame(GLFWwindow *window, Renderer *renderer){
    vkWaitForFences(renderer->device.device, 1, &renderer->inFlightFences[renderer->current_frame], VK_TRUE, UINT64_MAX);
    uint32_t imageIndex;

    VkResult result = vkAcquireNextImageKHR(
        renderer->device.device, 
        renderer->swap_chain.swapChain, 
        UINT64_MAX, 
        renderer->imageAvailableSemaphores[renderer->current_frame], 
        VK_NULL_HANDLE, 
        &imageIndex)
    ;


    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain(window, renderer);
        return NE_FAILURE;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }
    vkResetFences(renderer->device.device, 1, &renderer->inFlightFences[renderer->current_frame]);
    vkResetCommandBuffer(renderer->command_buffers[renderer->current_frame], /*VkCommandBufferResetFlagBits*/ 0);
    recordCommandBuffer(renderer, renderer->command_buffers[renderer->current_frame], imageIndex, renderer->pipelines["default"]);


    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    
    VkSemaphore waitSemaphores[] = {renderer->imageAvailableSemaphores[renderer->current_frame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &renderer->command_buffers[renderer->current_frame];

    VkSemaphore signalSemaphores[] = {renderer->renderFinishedSemaphores[renderer->current_frame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(renderer->device.graphics_queue, 1, &submitInfo, renderer->inFlightFences[renderer->current_frame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {renderer->swap_chain.swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(renderer->device.present_queue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || renderer->framebufferResized) {
        renderer->framebufferResized = false;
        recreateSwapChain(window, renderer);
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    renderer->current_frame = (renderer->current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
    

    return NE_SUCCESS;
}

uint32_t ne::create_command_buffer(Renderer *renderer){

    renderer->command_buffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = renderer->command_pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) renderer->command_buffers.size();

    if (vkAllocateCommandBuffers(renderer->device.device, &allocInfo, renderer->command_buffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
    return NE_SUCCESS;
}

uint32_t ne::create_command_pool(Renderer *renderer) {
    QueueFamilyIndices queueFamilyIndices = find_queue_families(renderer->device.physical_device, renderer->surface);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphics_family.value();

    if (vkCreateCommandPool(renderer->device.device, &poolInfo, nullptr, &renderer-> command_pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics command pool!");
    }

    return NE_SUCCESS;
}


uint32_t ne::create_frame_buffers(Renderer *renderer){
    renderer->frame_buffers.resize(renderer->swap_chain.image_views.size());

    for (size_t i = 0; i < renderer->swap_chain.image_views.size(); i++) {
        VkImageView attachments[] = {
            renderer->swap_chain.image_views[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderer->render_pass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = renderer->swap_chain.extent.width;
        framebufferInfo.height = renderer->swap_chain.extent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(renderer->device.device, &framebufferInfo, nullptr, &renderer->frame_buffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
    return NE_SUCCESS;
}

uint32_t ne::create_render_pass(ne::Renderer *renderer){
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = renderer -> swap_chain.image_format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(renderer->device.device, &renderPassInfo, nullptr, &renderer -> render_pass) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass!");
        }
    return NE_SUCCESS;
}


uint32_t ne::destroy_renderer(ne::Renderer renderer, bool enableValidationLayers){
    cleanupSwapChain(renderer);
    for (const auto& [name, pipeline] : renderer.pipelines){
        vkDestroyPipeline(renderer.device.device, pipeline, nullptr);
    }
    vkDestroyPipelineLayout(renderer.device.device, renderer.pipeline_layout, nullptr);
    vkDestroyRenderPass(renderer.device.device, renderer.render_pass, nullptr);
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(renderer.device.device, renderer.imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(renderer.device.device, renderer.renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(renderer.device.device, renderer.inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(renderer.device.device, renderer.command_pool, nullptr);

    ne::destroy_device(renderer.device);
    if(enableValidationLayers){
        DestroyDebugUtilsMessengerEXT(renderer.instance, renderer.debugMessenger, nullptr);
    }
    vkDestroySurfaceKHR(renderer.instance, renderer.surface, nullptr);
    vkDestroyInstance(renderer.instance, nullptr);

    return NE_SUCCESS;
}


std::vector<const char*> getRequiredExtensions(bool enableValidationLayers) {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}


uint32_t ne::create_instance(ne::Renderer *renderer, bool enableValidationLayers){

    if(enableValidationLayers && !ne::checkValidationLayerSupport()){
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = renderer -> application_name;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = renderer -> engine_name;
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;


    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getRequiredExtensions(enableValidationLayers);
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        ne::populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &renderer -> instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
    
    return NE_SUCCESS;
}


uint32_t ne::create_surface(GLFWwindow *window, VkInstance instance, VkSurfaceKHR *surface){
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
    return NE_SUCCESS;

}