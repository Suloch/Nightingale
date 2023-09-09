
#pragma once

#include<string>
#include "transform2D.hpp"

namespace nge{
    
    class Collider2D{
        Transform2D * transform;
        public:
            std::string type;

    };

    class Collision2D{
        Collider2D *col1;
        Collider2D *col2;
    };


    class BoxCollider2D: public Collider2D{
        
        private:
            float startX;
            float startY;
            float endX;
            float endY;

        public:

            BoxCollider2D(float startX, float startY, float endX, float endY);
            ~BoxCollider2D();

    };
} // namespace nge
