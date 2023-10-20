#include "render.hpp"
#include <stdexcept>
#include <glm/glm.hpp>
#include "../../logger/logger.hpp"

void nge::renderBuffer(
    Window *window,
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
){
    vkWaitForFences(device->device, 1, &pipeline->syncObjects->inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    for(int i; i < uts->size(); i++){
        Logger::getInstance().log(uts->at(i).texture, "  ",  uts->at(i).object);
        updateDescriptorSet(device->device, dSets[uts->at(i).object->name][currentFrame], textures[uts->at(i).texture], uts->at(i).object->buffer->uniformBuffer);

        uts->at(i).no++;
        if(uts->at(i).no == 2){
            uts->erase(uts->begin() + i);
        }
    }

    uint32_t imageIndex;

    VkResult result = vkAcquireNextImageKHR(
        device->device, 
        device->swapchain, 
        UINT64_MAX, 
        pipeline->syncObjects->imageAvailableSemaphores[currentFrame], 
        VK_NULL_HANDLE, 
        &imageIndex)
    ;
    
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        reCreateSwapChain(window, device, renderPass);
        return ;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }


    vkResetFences(device->device, 1,  &pipeline->syncObjects->inFlightFences[currentFrame]);
    vkResetCommandBuffer(command->buffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
    recordCommandBuffer(
        device,
        pipelineLayout,
        pipeline,
        command->buffers[currentFrame],
        renderPass,
        gameObjects,
        dSets,
        imageIndex,
        drawData,
        currentFrame
    );


    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    
    VkSemaphore waitSemaphores[] = {pipeline->syncObjects->imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &command->buffers[currentFrame];

    VkSemaphore signalSemaphores[] = {pipeline->syncObjects->renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;


    if (vkQueueSubmit(device->graphics, 1, &submitInfo, pipeline->syncObjects->inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {device->swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(device->present, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || device->framebufferResized) {
        device->framebufferResized = false;
        reCreateSwapChain(window, device, renderPass);
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

}

void nge::recordCommandBuffer(
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
){

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(cBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = device->frameBuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = device->extent;

    VkClearValue clearColor = {{{0.5f, 0.6f, 0.7f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(cBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(cBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)device->extent.width;
    viewport.height = (float)device->extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(cBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = device->extent;
    vkCmdSetScissor(cBuffer, 0, 1, &scissor);

    int i = 0;
    

    for(auto mapObject: gameObjects){
        auto buffer = mapObject.second->buffer;
        VkBuffer vertexBuffers[] = {buffer->vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(cBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(cBuffer, buffer->indexBuffer, 0, VK_INDEX_TYPE_UINT16);
        vkCmdBindDescriptorSets(cBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout->layout, 0, 1, &dSets[mapObject.second->name][currentFrame], 0, nullptr);
        Vertex2 temp{};
        temp.zoom = glm::float32(3);
        vkCmdPushConstants(cBuffer, pipelineLayout->layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Vertex2), &temp);
        vkCmdDrawIndexed(cBuffer, static_cast<uint32_t>(6), 1, 0, 0, 0);
        i++;
    }    

    // temp.pos = glm::vec2(x, 0);
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cBuffer);
    vkCmdEndRenderPass(cBuffer);

    if (vkEndCommandBuffer(cBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }

}

void nge::reCreateSwapChain(Window *window, Device *device, VkRenderPass renderPass){
    int width = 0, height = 0;
    glfwGetFramebufferSize(window->window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window->window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device->device);

    device->cleanSwapChain();
    SwapChainSupportDetails details = device->querySwapChainSupport(device->physical);
    QueueFamilyIndices indices = device->findQueueFamilyIndices(device->physical);
    device->swapchain = window->createSwapChain(
        device->physical,
        device->device,
        device->surface,
        details.capabilities,
        details.formats,
        details.present_modes,
        indices.graphicsFamily,
        indices.presentFamily
    );
    device->image_views = window->createImageViews(device->device);
    device->frameBuffers = window->createFrameBuffers(device->device, device->image_views, renderPass);

}
