
#include "nightingale.hpp"
#include "logger/logger.hpp"
#include "window/window.hpp"
#include "renderer/pass/renderpass.hpp"
#include "render/renderer/device/device.hpp"

#include<thread>
#include<chrono>

void nge::Nightingale::run(){
    nge::Logger::getInstance().log("Inside run");

}

nge::Nightingale::Nightingale(int height, int width, const char* name){
    // init logger
    nge::Logger::getInstance().start(nge::debug, "log.txt");

    std::vector<const char*>validationLayers;

    device = new Device(true, validationLayers);
    window = new Window(name, height, width);

    device->surface = window->createSurface(device->instance);

    device->create();

    SwapChainSupportDetails details = device->querySwapChainSupport();
    QueueFamilyIndices indices = device->findQueueFamilyIndices();

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

    renderpass = createRenderPass(
        device->device,
        window->format
    );
    device->frameBuffers = window->createFrameBuffers(device->device, device->image_views, renderpass);

    pipelineLayout = new PipelineLayout();

    command = new Command(device->device, indices.graphicsFamily);

    // syncObjects = new SyncObjects(MAX_FRAMES_IN_FLIGHT, device->device);

    // create a empty scene
    scenes["default"] = Scene("default");

    // create the default pipeline
    pipelines["default"] = new Pipeline(
            "default", 
            device->device, 
            pipelineLayout->layout,
            renderpass,
            nullptr,
            nullptr
        );
}

nge::Nightingale::~Nightingale(){
    
}

void nge::Nightingale::loadScene(std::string name){
    if(scenes.contains(name)){
        for(auto &object: scenes[name].gameObjects){
            //create vertex and index buffer for each of the object
            if(object.properties.contains("texture")){
                //render only if a texture property is present
                GameObjectBuffer *buffer = new GameObjectBuffer(
                    device->physical,
                    device->device,
                    device->graphics,
                    device->commandPool,
                    object.x,
                    object.y,
                    object.sx,
                    object.sy
                );
                buffer->texture = object.properties["texture"];
                buffers.push_back(buffer);
            }

        }
    }else{
        throw std::runtime_error("Could not find scene with that name");
    }

}

void nge::Nightingale::run(){
    while(!glfwWindowShouldClose(window->window)){
        glfwPollEvents();
        if(glfwGetKey(window->window, GLFW_KEY_W) == GLFW_PRESS){
        }
        for(auto &buffer: buffers){
            renderBuffer(window->window, pipelines["default"], buffer, dLayout);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void nge::Nightingale::createTexture(const char* name, const char* filepath){
    Texture *t = new Texture(device->device, name, filepath);
    textures[name] = t;
}