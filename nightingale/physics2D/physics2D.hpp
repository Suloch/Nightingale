
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
            void addBoxCollider2D(RigidBody2D *rb, Transform2D* transfrom, float centerX, float centerY, float height, float width);
            RigidBody2D* addRigidBody2D(Transform2D* transform);
            void detectCollisions();
            void resolveCollisions();
            
    };
}