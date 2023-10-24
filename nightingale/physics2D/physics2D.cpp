#include "physics2D.hpp"
#include "../logger/logger.hpp"

nge::Physics2D::Physics2D(){

}

nge::RigidBody2D* nge::Physics2D::addRigidBody2D(Transform2D *transform){

    RigidBody2D *rb = new RigidBody2D(transform);
    rbs.push_back(rb);
    return rb;
}

void nge::Physics2D::addBoxCollider2D(RigidBody2D *rb, Transform2D* transfrom, float centerX, float centerY, float height, float width){
    BoxCollider2D *bc = new BoxCollider2D(transfrom, rb, centerX, centerY, height, width);
    this->colliders.push_back(bc);
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

    for(int i=0; i<2; i++){

        std::vector<Collision2D *> collisions;
        for(auto rb: rbs){
            rb->update(dt);
        }

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
    }


}


