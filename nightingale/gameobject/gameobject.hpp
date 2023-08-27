#pragma once

#include <map>
#include <string>

namespace nge{
    class GameObject{
        public:
            float x; //x-position
            float y; //y-position
            float sx; //x-scale
            float sy; //y-scale
            float r; //rotation in radians
            GameObject();
            ~GameObject();
        
        private:
            char * name;
            std::map<std::string, std::string> properties;
        
    };
} // namespace nge
