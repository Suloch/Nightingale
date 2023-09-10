#include "physics2D.hpp"


nge::Physics2D::Physics2D(){

}

void nge::Physics2D::addRigidBody2D(Transform2D *transform){

    RigidBody2D *rb = new RigidBody2D(transform);
    rbs.push_back(rb);
}


nge::Physics2D::~Physics2D(){
    for(auto rb: rbs){
        delete rb;
    }
    for(auto col: colliders){
        delete col;
    }
}

void nge::Physics2D::step(float dt){

    // detectCollisions();
    // resolveCollisions();

    for(auto rb: rbs){
        rb->update(dt);
    }
}
