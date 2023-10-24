
#include "rigidbody2d.hpp"
#include "../logger/logger.hpp"

nge::RigidBody2D::RigidBody2D(Transform2D *transform){
    this->transform = transform;
    this->mass = 1;
    this->gravity = 10;
    this->velocityX = 0;
    this->velocityY = 0;
    this->dynamic = true;
}

void nge::RigidBody2D::update(float dt){
    if(!this->dynamic)
        return;
    //update velocity

    //update position
    // transform->x = transform->x + velocityX * dt + 0.5 * gravity * dt * dt;
    transform->y = transform->y + velocityY * dt + 0.5 * gravity * dt * dt;

    velocityY += gravity * dt;
    // Logger::getInstance().log("Velocity: ", velocityY, " X: ", transform->x, " Y: ", transform->y);
}