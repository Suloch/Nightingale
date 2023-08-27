
#include "nightingale.hpp"
#include "logger/logger.hpp"



void nge::Nightingale::run(){
    nge::Logger::getInstance().log("Inside run");

}

nge::Nightingale::Nightingale(int height, int width, const char* name){

    VkInstance instance;
    VkSurfaceKHR surface;
    
    std::vector<const char*>validationLayers;


    this->device = Device(instance, surface, true, validationLayers);

    // init logger
    nge::Logger::getInstance().start(nge::debug, "log.txt");

    // create a empty scene
    scenes["default"] = Scene("default");
}

nge::Nightingale::~Nightingale(){
    
}


void nge::Nightingale::createTexture(const char* name, const char* filepath){
    textures.push_back(Texture(device, name, filepath));
}