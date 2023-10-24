
#pragma once

#include<string>
#include "transform2D.hpp"
#include "rigidbody2d.hpp"
#include <vector>

namespace nge{
    enum ColliderType{
        BOX
    };

    class Collision2D{

        public:
            void *col1;
            void *col2;

            float distX;
            float distY;

            typedef  void(*SolverCallback)(Collision2D*);
            std::vector<SolverCallback> solvers;

            Collision2D(void *col1, void *col2, float distx, float distY);
            void resolve();
    };

    class Collider2D{
        public:
            RigidBody2D *rb;
            ColliderType type;
            virtual Collision2D * test(Collider2D *){return nullptr;}

    };



    class BoxCollider2D: public Collider2D{
        public:
            float centerX;
            float centerY;
            float height;
            float width;
            Collision2D * test(Collider2D *);
            BoxCollider2D(Transform2D* t, RigidBody2D *rb,  float centerX, float centerY, float height, float width);

    };
    void positionSolver(Collision2D*);
    
    Collision2D *testBox_Box(BoxCollider2D* box1, BoxCollider2D * box2);
} // namespace nge
