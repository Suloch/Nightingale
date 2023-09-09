
#pragma once

#include "transform2D.hpp"

namespace nge{
    class RigidBody2D{
        private:
            Transform2D * transform;

        public:
            float mass;
            float gravity;
            float velocity;
            bool dynamic;
    };
}