
#include "gameobject.hpp"

nge::GameObject::GameObject(){
    x = 0;
    y = 0;
    sx = 1;
    sy = 1;
    r = 0;
    texScale = 1.0f;
    texOffsetX = 0.0f;
    texOffsetY = 0.0f;
    name = "GameObject";
}

nge::GameObject::GameObject(std::string name){
    x = 0;
    y = 0;
    sx = 1;
    sy = 1;
    r = 0;
    texScale = 1.0f;
    texOffsetX = 0.0f;
    texOffsetY = 0.0f;
    this->name = name;
}

nge::GameObject::GameObject(std::string name, float x, float y){
    GameObject();
    this->name = name;
    this->x = x;
    this->y = y;
}

void nge::GameObject::setScale(float scale){
    sx = scale;
    sy = scale;
}

void nge::GameObject::setTexScale(float scale, float offsetX, float offsetY){
    this->texScale = scale;
    this->texOffsetX = offsetX;
    this->texOffsetY = offsetY;
}

void nge::GameObject::setPosition(float x, float y){
    this->currX = x;
    this->currY = y;
}

nge::GameObject::~GameObject(){
    
}