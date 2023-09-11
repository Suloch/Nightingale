


#include "collider2D.hpp"

nge::Collision2D::Collision2D(void *col1, void *col2){
    this->col1 = col1;
    this->col2 = col2;
}

void nge::Collision2D::resolve(){
    Collider2D * col1 = static_cast<Collider2D *>(this->col1);
    Collider2D * col2 = static_cast<Collider2D *>(this->col2);

    //resolve
}


nge::BoxCollider2D::BoxCollider2D(float centerX, float centerY, float height, float width){
    this->type = BOX;
    this->centerX = centerX;
    this->centerY = centerY;
    this->height = height;
    this->width = width;
}

nge::Collision2D* nge::BoxCollider2D::test(nge::Collider2D *col){
    switch (col->type)
    {
    case BOX:
        BoxCollider2D * bc = static_cast<BoxCollider2D *>(col);
        
        return testBox_Box(this, bc);
    
    default:
        break;
    }
}

nge::Collision2D* nge::testBox_Box(nge::BoxCollider2D* b1, nge::BoxCollider2D* b2){
    if(b1->centerX-b2->centerX < b1->width/2 + b2->width/2){
        return new Collision2D(b1, b2);
    }
    if(b1->centerY-b2->centerY < b1->height/2 + b2->height/2){
        return new Collision2D(b1, b2);
    }
}