
#include "nightingale.hpp"
#include "logger/logger.hpp"
#include "renderer/pass/renderpass.hpp"
#include "third-party/imgui/interface.hpp"
#include "../levelbuilder/commandqueue.hpp"


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
    
    interface = new Interface(
        device->device, 
        window->window, 
        device->physical, 
        device->instance, 
        device->graphics, 
        renderpass,
        command->pool
    );

    this->camera = new Camera2D(this->device->extent.height, this->device->extent.width);
    
}

nge::Nightingale::~Nightingale(){
    vkDeviceWaitIdle(device->device);
    delete interface;
    for(auto &texture: textures){
        delete texture.second;
    }
    
    vkDestroyDescriptorPool(device->device, dPool, nullptr);

    for(auto pipeline: pipelines){
        delete pipeline.second;
    }

    delete pipelineLayout;

    vkDestroyRenderPass(device->device, renderpass, nullptr);
    
    delete command;
    
    delete window;
    
    delete device;

    delete camera;

}

void nge::Nightingale::loadScene(std::string name){
    if(scenes.contains(name)){
        currentSceneName = name;
        for(auto mapObject: scenes[name].gameObjects){
            auto object = mapObject.second;
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
                for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
                    dSets[object->name].push_back(createDescriptorset(device->device, dPool, pipelineLayout->dSet));
                    updateDescriptorSet(device->device, dSets[object->name][i], textures[buffer->texture], buffer->uniformBuffer);
                }
                object->buffer = buffer;
            }

        }
    }else{
        throw std::runtime_error("Could not find scene with that name");
    }

}

void nge::Nightingale::run(){
    currentFrame = 1; 
    float x = 0;
    float y = 0;
    float z = 1.5;
    float k = -100;
    float l = 0 ;
    while(!glfwWindowShouldClose(window->window)){

        glfwPollEvents();
        if(glfwGetKey(window->window, GLFW_KEY_W) == GLFW_PRESS){
            x-=0.1;
        }
        if(glfwGetKey(window->window, GLFW_KEY_S) == GLFW_PRESS){
            x+=0.1;
        }
        if(glfwGetKey(window->window, GLFW_KEY_D) == GLFW_PRESS){
            y-=0.1;
        }
        if(glfwGetKey(window->window, GLFW_KEY_A) == GLFW_PRESS){
            y+=0.1;
        }
        camera->setPosition(y, x);
        

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();


        if(this->editorMode){

            // interface->showEditorInterface(textures, scenes["default"].gameObjects);
            handleCommands();

        }
        // ImGui::ShowDemoWindow();

        ImGui::Render();
        ImDrawData* drawData = ImGui::GetDrawData();

        for(auto object: scenes[currentSceneName].gameObjects){
            object.second->buffer->updateUniformBuffer(device->extent, camera, textures[object.second->buffer->texture]->getAspectRatio());
        }

        renderBuffer(
            window,
            device,
            command,
            pipelineLayout,
            pipelines["default"],
            renderpass, 
            scenes[currentSceneName].gameObjects,
            dSets,
            currentFrame,
            drawData,
            textures,
            &uts
        );
        
        
        physic2d.step(std::chrono::milliseconds(20).count()/100.0f);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
        delete gameobject.second;
    }
}


void nge::Nightingale::setEditorMode(bool val){
    this->editorMode = val;
}

void nge::Scene::addGameObject(GameObject *object){
    object->id = this->gameObjects.size() + 1;
    if(object->name == ""){
        object->name = "GameObject " + std::to_string(object->id);
    }
    this->gameObjects[std::to_string(object->id)] = object;
}

void nge::Nightingale::createObject(std::string name){

    GameObject * gameobject = new GameObject(name, device->extent.height/2 ,device->extent.width/2);
    scenes["default"].addGameObject(gameobject);
}

void nge::Nightingale::handleCommands(){
    level::Command command = level::CommandQueue::getInstance().getCommand();
    switch (command.type)
    {   case level::CREATE_TEXTURE:
            createTexture(command.data[0].c_str(), command.data[1].c_str());
            break;
        case level::NULL_COMMAND:
            break;
        case level::CREATE_OBJECT:
            createObject(command.data[0]);
            break;
        case level::UPDATE_TEXTURE:
            updateTexture(command.data[0], command.data[1]);
            break;
        case level::STOP_EXECUTE:
            return;
    }
}

void nge::Nightingale::updateTexture(std::string texName, std::string objId){
    changed = true;
    UpdateTexture ut;
    ut.object = this->scenes[currentSceneName].gameObjects[objId];
    ut.no = 0;
    ut.texture = name;
    uts.push_back(ut);
}