
#include "gameobject.hpp"
#include "../logger/logger.hpp"

nge::GameObject::GameObject(std::string name, int height, int width){
    this->name = name;
    transform = new Transform2D(height, width);

    mesh.indices = {
            0, 1, 2, 2, 3, 0
    };
    texScale = 1.0f;
    texOffsetX = 0.0f;
    texOffsetY = 0.0f;

    this->flipX = 1;
    this->flipY = 1;
    name = name;
    buffer = nullptr;
}

void nge::GameObject::setTexScale(float scale, float offsetX, float offsetY){
    this->texScale = scale;
    this->texOffsetX = offsetX;
    this->texOffsetY = offsetY;
}


nge::GameObject::~GameObject(){
    delete transform;
    delete buffer;
}