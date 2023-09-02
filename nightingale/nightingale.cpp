
#include "nightingale.hpp"
#include "logger/logger.hpp"
#include "renderer/pass/renderpass.hpp"

#include<thread>
#include<chrono>


nge::Nightingale::Nightingale(int height, int width, const char* name){
    // init logger
    nge::Logger::getInstance().start(nge::debug, "log.txt");

    std::vector<const char*>validationLayers;

    device = new Device(true, validationLayers, getRequiredExtensions(true));
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

    dPool = createDescriptorPool(device->device, MAX_FRAMES_IN_FLIGHT);
    dLayout = createDescriptorLayout(device->device);

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
                buffer->object = &object;
                buffers.push_back(buffer);
            }

        }
    }else{
        throw std::runtime_error("Could not find scene with that name");
    }

}

void nge::Nightingale::run(){
    currentFrame = 1; 
    while(!glfwWindowShouldClose(window->window)){
        glfwPollEvents();
        if(glfwGetKey(window->window, GLFW_KEY_W) == GLFW_PRESS){
        }
        for(auto &buffer: buffers){
            renderBuffer(
                window,
                device,
                command,
                pipelineLayout,
                pipelines["default"],
                renderpass, 
                buffer,
                dSets[buffer->texture],
                currentFrame
            );
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void nge::Nightingale::createTexture(const char* name, const char* filepath){
    Texture *t = new Texture(device->graphics, device->physical, device->device, name, filepath);
    dSets[name] = createDescriptorsets(MAX_FRAMES_IN_FLIGHT, device->device, dPool, dLayout, t);
}


nge::Scene::Scene(const char *name){
    this->name = name;
}

nge::Scene::~Scene(){

}