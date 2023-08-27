
#include "gameobject.hpp"

nge::GameObject::GameObject(){
    x = 0;
    y = 0;
    sx = 1;
    sy = 1;
    r = 0;
    name = "GameObject";
}

nge::GameObject::GameObject(char *name){
    GameObject();
    this->name = name;
}

nge::GameObject::GameObject(char *name, float x, float y){
    GameObject();
    this->name = name;
    this->x = x;
    this->y = y;
}

nge::GameObject::~GameObject(){
    
}