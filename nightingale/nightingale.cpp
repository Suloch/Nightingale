
#include "nightingale.hpp"
#include "render/render.hpp"
#include "window/window.hpp"
#include "constants.hpp"

#include <cstdlib>



uint32_t ne::create_application(ne::Application *app){
    WindowCreateInfo windowInfo{};
    windowInfo.height = 800;
    windowInfo.width = 600;
    windowInfo.title = "Test";

    ne::create_window(&windowInfo, app -> window);



    return NE_SUCCESS;
}


uint32_t ne::run_application(ne::Application app){
    while(!glfwWindowShouldClose(app.window)){
        glfwPollEvents();
    }
    return NE_SUCCESS;
}

uint32_t ne::destroy_application(ne::Application app){

    ne::destroy_window(app.window);
    return NE_SUCCESS;
}