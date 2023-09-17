

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
    app->loadScene(this->sceneName);


}

void nge::LevelBuilder::run(){
    app->run();
}

nge::LevelBuilder::~LevelBuilder(){
    delete app;
}
