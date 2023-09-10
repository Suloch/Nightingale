
#pragma once

#include "transform2D.hpp"

namespace nge{
    class RigidBody2D{
        private:
            Transform2D * transform;

        public:
            float mass;
            float gravity;
            float velocityX;
            float velocityY;
            bool dynamic;
            RigidBody2D(Transform2D *t);
            ~RigidBody2D(){}
            void update(float dt);
    };
}