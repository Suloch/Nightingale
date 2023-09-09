
#include "nightingale.hpp"
#include "logger/logger.hpp"
#include "renderer/pass/renderpass.hpp"

#include<thread>
#include<chrono>


nge::Nightingale::Nightingale(int height, int width, const char* name){
    // init logger
    Logger::getInstance().start(nge::debug, "log.txt");

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    window = new Window(name, height, width);

    device = new Device(true, validationLayers, getRequiredExtensions(true));

    device->surface = window->createSurface(device->instance);

    device->create();

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
    device->extent = window->extent;

    renderpass = createRenderPass(
        device->device,
        window->format
    );
    device->frameBuffers = window->createFrameBuffers(device->device, device->image_views, renderpass);

    pipelineLayout = new PipelineLayout(device->device);

    command = new Command(device->device, indices.graphicsFamily);

    dPool = createDescriptorPool(device->device, MAX_FRAMES_IN_FLIGHT);

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
    vkDeviceWaitIdle(device->device);
    for(auto &texture: textures){
        delete texture.second;
    }
    // for(auto dset: dSets){
    //     for(auto &d: dset.second){
    //         vkDestroyDescriptorSet
    //     }
    // }
    
    vkDestroyDescriptorPool(device->device, dPool, nullptr);

    for(auto buffer: buffers){
        delete buffer;
    }

    for(auto pipeline: pipelines){
        delete pipeline.second;
    }

    delete pipelineLayout;

    vkDestroyRenderPass(device->device, renderpass, nullptr);
    
    delete command;
    
    delete window;
    
    delete device;

}

void nge::Nightingale::loadScene(std::string name){
    if(scenes.contains(name)){
        for(auto object: scenes[name].gameObjects){
            // create physics properties

            //create vertex and index buffer for each of the object
            if(object->properties.contains("texture")){
                //render only if a texture property is present
                GameObjectBuffer *buffer = new GameObjectBuffer(
                    device->physical,
                    device->device,
                    device->graphics,
                    command->pool
                );
                buffer->texture = object->properties["texture"];
                buffer->object = object;
                buffers.push_back(buffer);

                dSets[object->name] = createDescriptorset(device->device, dPool, pipelineLayout->dSet);
                updateDescriptorSet(device->device, dSets[object->name], textures[buffer->texture], buffer->uniformBuffer);

            }

        }
    }else{
        throw std::runtime_error("Could not find scene with that name");
    }

}

void nge::Nightingale::run(){
    currentFrame = 1; 
    float x = 0;
    float y = 0.0000000001;
    float z = 1.5;
    float k = -100;
    float l = 0 ;
    while(!glfwWindowShouldClose(window->window)){
        glfwPollEvents();
        if(glfwGetKey(window->window, GLFW_KEY_W) == GLFW_PRESS){
            k+=10;
        }
        if(glfwGetKey(window->window, GLFW_KEY_S) == GLFW_PRESS){
            k-=10;
        }
        if(glfwGetKey(window->window, GLFW_KEY_D) == GLFW_PRESS){
            l+=10;
        }
        if(glfwGetKey(window->window, GLFW_KEY_A) == GLFW_PRESS){
            l-=10;
        }
        // scenes["default"].gameObjects[6]->transform->scaleX = k;
        scenes["default"].gameObjects[6]->transform->x = l;
        scenes["default"].gameObjects[6]->transform->y = k;
        // Logger::getInstance().log(l, ": ", k);
    
        for(auto buffer: buffers){
            buffer->updateUniformBuffer(device->extent, x, y, z, textures[buffer->texture]->getAspectRatio());
        }

        renderBuffer(
            window,
            device,
            command,
            pipelineLayout,
            pipelines["default"],
            renderpass, 
            buffers,
            dSets,
            currentFrame
        );

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }
}

void nge::Nightingale::createTexture(const char* name, const char* filepath){
    Texture *t = new Texture(command->pool, device->graphics, device->physical, device->device, name, filepath);
    textures[name] = t;
}


nge::Scene::Scene(const char *name){
    this->name = name;
    
}

nge::Scene::~Scene(){
    for(auto gameobject: gameObjects){
        delete gameobject;
    }
}