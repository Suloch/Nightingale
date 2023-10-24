
#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../logger/logger.hpp"

nge::Camera2D::Camera2D(float height, float width){
    this->x = 0;
    this->y = yoffset;
    this->z = 1;
    this->tx = 0;
    this->ty = 0;
    this->tz = 0;
    
    this->perspective = 45.0f;

    this->height = height;
    this->width = width;
}


glm::mat4 nge::Camera2D::getView(){
    return glm::lookAt(
        glm::vec3(x, y, z),
        glm::vec3(tx,  ty, tz), 
        glm::vec3(0.0f, 0.0f, 1.0f)
        );
}

glm::mat4 nge::Camera2D::getProjection(){
    return glm::perspective(glm::radians(perspective), width / (float) height, 0.1f, 10.0f);
}

void nge::Camera2D::setPosition(float x, float y){
    this->x = x;
    this->y = y + yoffset;
    this->tx = x;
    this->ty = y;
}

nge::Camera2D::~Camera2D(){

}