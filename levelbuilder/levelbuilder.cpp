

#include "levelbuilder.hpp"


nge::LevelBuilder::LevelBuilder(){
    height = 768;
    width = 1376;

    app = new Nightingale(height, width, "Level Editor");
    app->setEditorMode(true);
    
    this->sceneName = "default";
    app->loadScene("default");


}

void nge::LevelBuilder::run(){
    app->run();
}

nge::LevelBuilder::~LevelBuilder(){
    delete app;
}