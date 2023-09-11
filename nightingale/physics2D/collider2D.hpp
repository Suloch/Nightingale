
#pragma once

#include<string>
#include "transform2D.hpp"

namespace nge{
    enum ColliderType{
        BOX
    };
    class Collision2D{
        void *col1;
        void *col2;

        public:
            Collision2D(void *col1, void *col2);
            void resolve();
    };

    class Collider2D{
        Transform2D * transform;
        public:
            ColliderType type;
            virtual Collision2D * test(Collider2D *);

    };



    class BoxCollider2D: public Collider2D{
        

        public:
            float centerX;
            float centerY;
            float height;
            float width;
            Collision2D * test(Collider2D *);
            BoxCollider2D(float centerX, float centerY, float height, float width);

    };
    Collision2D *testBox_Box(BoxCollider2D* box1, BoxCollider2D * box2);
} // namespace nge
