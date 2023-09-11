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
    std::vector<Collision2D *> collisions;

    for(int i=0; i<colliders.size(); i++){
        for(int j=i+1; j<colliders.size(); j++){
            Collision2D* col = colliders[i]->test(colliders[j]);
            if(col != nullptr){
                collisions.push_back(col);
            }
        }
    }

    // resolveCollisions();
    for(auto collision: collisions){
        collision->resolve();
        delete collision;
    }

    for(auto rb: rbs){
        rb->update(dt);
    }
}


