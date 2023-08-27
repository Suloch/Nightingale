#pragma once

#include "gameobject/gameobject.hpp"
#include<map>
#include<string>
namespace nge{
    class Scene{
        public:
            Scene(const char* name);
            ~Scene();
            void addObject(GameObject object);
            void removeObject(GameObject object);

        private:
            const char *name;
    };

    class Nightingale{
    

        public:
            std::map<std::string, Scene> scenes;
            
            Nightingale(int height, int width, const char* name);
            ~Nightingale();

            void run();

        private:
            const char *name;
    };
    
}