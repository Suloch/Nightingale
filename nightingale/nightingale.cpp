
#include "nightingale.hpp"
#include "render/render.hpp"
#include "window/window.hpp"
#include "constants.hpp"
#include "render/pipeline.hpp"
#include "render/descripter_set.hpp"
#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>

uint32_t ne::create_application(ne::Application *app){
    WindowCreateInfo windowInfo{};
    windowInfo.height = 600;
    windowInfo.width = 800;
    windowInfo.title = "Test";

    app -> renderer.engine_name = NE_NAME;
    app -> renderer.application_name = "Test";
    ne::create_window(&windowInfo, &app -> window);
    ne::create_renderer(app ->window, &app -> renderer, true);
    VkPipeline pipeline = ne::create_graphice_pipline(
            app->renderer.device.device, 
            app->renderer.render_pass, 
            app->renderer.pipeline_layout
        );
    
    
    // VkPipeline pipeline = ne::create_piepline2(app->renderer.device.device, app->renderer.render_pass);
    app->renderer.pipelines.insert({"default", pipeline});

    // glfwSetKeyCallback(app->window, key_callback);
    return NE_SUCCESS;
}


uint32_t ne::run_application(ne::Application *app){
    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };
    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };
    Buffer vertex_buffer = createVertexBuffer(app->renderer.command_pool, app->renderer.device, vertices);
    Buffer index_buffer = createIndexBuffer(app->renderer.command_pool, app->renderer.device, indices);
    Texture texture = create_texture(app->renderer.device, app->renderer.command_pool);
    VkDescriptorPool descriptor_pool = createDescriptorPool(MAX_FRAMES_IN_FLIGHT, app->renderer.device);
    VkDescriptorSetLayout layout;
    create_graphics_set_layout(app->renderer.device.device, &layout);

    std::vector<VkDescriptorSet> descriptorSets = createDescriptorSets(MAX_FRAMES_IN_FLIGHT, app->renderer.device, descriptor_pool, texture, layout);
    float x = 0;
    while(!glfwWindowShouldClose(app->window)){
        glfwPollEvents();
        if(glfwGetKey(app->window, GLFW_KEY_W) == GLFW_PRESS){
            x += 0.1;
        }
        ne::render_frame(app->window, &app->renderer, vertex_buffer, index_buffer, descriptorSets, x);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    vkDestroyBuffer(app->renderer.device.device, vertex_buffer.buffer, nullptr);
    vkFreeMemory(app->renderer.device.device, vertex_buffer.bufferMemory, nullptr);
    destroy_texture(app->renderer.device, texture);

    vkDeviceWaitIdle(app->renderer.device.device);
    return NE_SUCCESS;
}

uint32_t ne::destroy_application(ne::Application app){
    ne::destroy_renderer(app.renderer, true);
    ne::destroy_window(app.window);
    return NE_SUCCESS;
}