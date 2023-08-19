
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
    app->pipeline = ne::create_graphice_pipline(
            app->renderer.device.device, 
            app->renderer.render_pass, 
            app->renderer.pipeline_layout
        );

    return NE_SUCCESS;
}


uint32_t ne::run_application(ne::Application app){
    
    while(!glfwWindowShouldClose(app.window)){
        glfwPollEvents();
        
        ne::render_frame(app.window, &app.renderer, app.pipeline);
    }
    vkDeviceWaitIdle(app.renderer.device.device);
    return NE_SUCCESS;
}

uint32_t ne::destroy_application(ne::Application app){
    vkDestroyPipeline(app.renderer.device.device, app.pipeline, nullptr);
    ne::destroy_renderer(app.renderer, true);
    ne::destroy_window(app.window);
    return NE_SUCCESS;
}