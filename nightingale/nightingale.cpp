
#include "nightingale.hpp"
#include "render/render.hpp"
#include "window/window.hpp"
#include "constants.hpp"

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
    ne::create_renderer(&app -> renderer, true);


    return NE_SUCCESS;
}


uint32_t ne::run_application(ne::Application app){
    
    while(!glfwWindowShouldClose(app.window)){
        glfwPollEvents();
    }
    return NE_SUCCESS;
}

uint32_t ne::destroy_application(ne::Application app){
    ne::destroy_renderer(app.renderer, true);
    ne::destroy_window(app.window);
    return NE_SUCCESS;
}