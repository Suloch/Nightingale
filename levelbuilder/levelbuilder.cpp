

#include "levelbuilder.hpp"
#include "commandqueue.hpp"
#include <iostream>
#include <thread>
#include "../nightingale/logger/logger.hpp"

nge::LevelBuilder::LevelBuilder(){
    height = 768;
    width = 1376;

    app = new Nightingale(height, width, "Level Editor");
    app->setEditorMode(true);
    
    this->sceneName = "default";
    app->loadScene("default");


}

void nge::LevelBuilder::run(){
    commandThread= std::thread(&LevelBuilder::runCommands, this);
    app->run();
}

nge::LevelBuilder::~LevelBuilder(){
    delete app;
    commandThread.join();
}


void nge::LevelBuilder::runCommands(){
    level::Command command = level::CommandQueue::getInstance().getCommand();

    while(true){
        switch (command.type)
        {
            case level::CREATE_TEXTURE:
                break;
            case level::NULL_COMMAND:
                break;;
            case level::STOP_EXECUTE:
                return;
        }
        command = level::CommandQueue::getInstance().getCommand();
    }

}