
#include "nightingale.hpp"
#include "render/render.hpp"
#include "window/window.hpp"
#include "constants.hpp"
#include "render/pipeline.hpp"

#include <cstdlib>
#include <iostream>


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

    while(!glfwWindowShouldClose(app->window)){
        glfwPollEvents();
        if(glfwGetKey(app->window, GLFW_KEY_W) == GLFW_PRESS){
            std::cout<<"hello"<<std::endl;
        }
        ne::render_frame(app->window, &app->renderer);

    }
    vkDeviceWaitIdle(app->renderer.device.device);
    return NE_SUCCESS;
}

uint32_t ne::destroy_application(ne::Application app){
    ne::destroy_renderer(app.renderer, true);
    ne::destroy_window(app.window);
    return NE_SUCCESS;
}