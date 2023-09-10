
#pragma once

#include "collider2D.hpp"
#include "rigidbody2d.hpp"

#include <vector>

namespace nge{

    
    
    class Physics2D{

        std::vector<RigidBody2D *> rbs;         
        std::vector<Collider2D *> colliders;

        
        public:
            Physics2D();
            ~Physics2D();

            
            void step(float dt);
            void addBoxCollider2D(Transform2D* transfrom, int startX, int startY, int endX, int endY);
            void addRigidBody2D(Transform2D* transform);
            void detectCollisions();
            void resolveCollisions();
            
    };
}