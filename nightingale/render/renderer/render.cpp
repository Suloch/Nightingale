#include "render.hpp"
#include <stdexcept>
#include <glm/glm.hpp>

void nge::renderBuffer(
    VkDevice device,

){
    vkWaitForFences(device, 1, &renderer->inFlightFences[renderer->current_frame], VK_TRUE, UINT64_MAX);
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
    recordCommandBuffer(renderer, renderer->command_buffers[renderer->current_frame], imageIndex, renderer->pipelines["default"], vertex_buffer, index_buffer, descriptorSets, x);


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

void nge::recordCommandBuffer(
    Device *device, 
    PipelineLayout *pipelineLayout, 
    Pipeline *pipeline, 
    VkCommandBuffer cBuffer,  
    VkRenderPass renderPass,
    uint32_t imageIndex,
    GameObjectBuffer buffer

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

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
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

    VkBuffer vertexBuffers[] = {buffer.vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(cBuffer, 0, 1, vertexBuffers, offsets);
    
    vkCmdBindIndexBuffer(cBuffer, buffer.indexBuffer, 0, VK_INDEX_TYPE_UINT16);
    vkCmdBindDescriptorSets(cBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout->layout, 0, 1, &descriptorSets[frameIndex], 0, nullptr);

    Vertex2 temp{};
    // temp.pos = glm::vec2(x, 0);
    temp.zoom = glm::float32(3);
    vkCmdPushConstants(cBuffer, pipelineLayout->layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Vertex2), &temp);
    vkCmdDrawIndexed(cBuffer, static_cast<uint32_t>(6), 1, 0, 0, 0);
    vkCmdEndRenderPass(cBuffer);

    if (vkEndCommandBuffer(cBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }

}