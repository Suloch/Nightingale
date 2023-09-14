


#include "collider2D.hpp"
#include "../logger/logger.hpp"

nge::Collision2D::Collision2D(void *col1, void *col2, float x, float y){
    this->col1 = col1;
    this->col2 = col2;
    this->distX = x;
    this->distY = y;
    this->solvers.push_back(positionSolver);
}

void nge::Collision2D::resolve(){
    

    //resolve
    for(auto solver: solvers){
        solver(this);
    }
}


nge::BoxCollider2D::BoxCollider2D(Transform2D* t, RigidBody2D *rb, float centerX, float centerY, float height, float width){
    this->rb = rb;
    this->type = BOX;
    this->centerX = centerX;
    this->centerY = centerY;
    this->height = height;
    this->width = width;
}

nge::Collision2D* nge::BoxCollider2D::test(Collider2D *col){
    switch (col->type)
    {
    case BOX:
        BoxCollider2D * bc = static_cast<BoxCollider2D *>(col);
        
        return testBox_Box(this, bc);
    }
}

nge::Collision2D* nge::testBox_Box(nge::BoxCollider2D* b1, nge::BoxCollider2D* b2){
    float distX = b1->rb->transform->x-b2->rb->transform->x ;
    float distY = b1->rb->transform->y-b2->rb->transform->y ;

    float xdiff = b1->width/2 + b2->width/2 - std::abs(distX);
    float ydiff = b1->height/2 + b2->height/2 - std::abs(distY) ;
    
    if( xdiff > 0  && ydiff > 0){
        return new Collision2D(b1, b2, distX, ydiff);
        
    }
    return nullptr;
}

void nge::positionSolver(nge::Collision2D *col){
    Collider2D * col1 = static_cast<Collider2D *>(col->col1);
    Collider2D * col2 = static_cast<Collider2D *>(col->col2);
    

    col1->rb->velocityY = 0;
    col1->rb->velocityX = 0;

    col2->rb->velocityX = 0;
    col2->rb->velocityY = 0;
    const float min = 0.1;

   

    if(col1->rb->dynamic){

        col1->rb->transform->y += col->distY/2;
        col1->rb->transform->x += col->distX/2;
    }
    if(col2->rb->dynamic){
        col2->rb->transform->y -= col->distY/2;
        col2->rb->transform->x -= col->distX/2;
    }


}