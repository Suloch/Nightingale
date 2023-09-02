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
            std::map<std::string, std::string> properties;

            GameObject();
            GameObject(char *name);
            GameObject(char *name, float x, float y);
            ~GameObject();
        
        private:
            char * name;
        
    };
} // namespace nge
