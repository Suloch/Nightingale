
#include "gameobject.hpp"
#include "../logger/logger.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<cstring>

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

void nge::GameObject::updateUniformBuffer(VkExtent2D extent, Camera2D *camera, float ar){

    UniformBufferObject ubo{};
    
   
    ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(transform->getX(), transform->getY(), transform->getZ()));
    ubo.model = glm::scale(ubo.model, glm::vec3(transform->scaleX, transform->scaleY, 1));
    ubo.model = glm::rotate(ubo.model, glm::radians(transform->rot), glm::vec3(0.0f, 0.0f, 1.0f));
    // ubo.model = glm::mat4(1.0f);
    ubo.view = camera->getView();
    ubo.proj = camera->getProjection();
    ubo.proj[1][1] *= -1;
    // ubo.tex = glm::mat2(1.0f) * glm::mat2();

    ubo.tbo.flipX = this->flipX;
    ubo.tbo.flipY = this->flipY;
    ubo.tbo.scale = this->texScale;
    ubo.tbo.ar = ar*transform->scaleX/transform->scaleY;
    ubo.tbo.x = this->texOffsetX;
    ubo.tbo.y = this->texOffsetY;
    memcpy(this->buffer->uniformBufferMapped, &ubo, sizeof(ubo));
}