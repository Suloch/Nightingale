
#include "rigidbody2d.hpp"
#include "../logger/logger.hpp"

nge::RigidBody2D::RigidBody2D(Transform2D *transform){
    this->transform = transform;
    this->mass = 1;
    this->gravity = 10;
    this->velocityX = 0;
    this->velocityY = 0;
}

void nge::RigidBody2D::update(float dt){
    //update velocity
    velocityY += gravity * dt;

    //update position
    transform->x += velocityX * dt;
    transform->y += velocityY * dt;

    // Logger::getInstance().log("Velocity: ", velocityY, " X: ", transform->x, " Y: ", transform->y);
}