#pragma once

#include <map>
#include <string>

namespace nge{
    class GameObject{
        public:
            float x; //x-position
            float y; //y-position
            float sx = 1.0f; //x-scale
            float sy = 1.0f; //y-scale
            float r; //rotation in radians
            float texScale;
            float texOffsetX;
            float texOffsetY;
            float currX = 0;
            float currY = 0;
            std::map<std::string, std::string> properties;
            std::string name;

            GameObject();
            GameObject(std::string name);
            GameObject(std::string name, float x, float y);
            ~GameObject();
            void setScale(float scale);
            void setTexScale(float scale, float x, float y);
            void setPosition(float x, float y);
        private:
        
    };
} // namespace nge
