#pragma once

#include "gameobject/gameobject.hpp"
#include "texture/texture.hpp"

#include<map>
#include<string>
#include<vector>

namespace nge{
    class Scene{
        public:
            Scene(){}
            Scene(const char* name);
            ~Scene();
            std::vector<GameObject> gameObjects;

        private:
            const char *name;
    };

    class Nightingale{
    

        public:
            std::map<std::string, Scene> scenes;
            
            Nightingale(int height, int width, const char* name);
            ~Nightingale();

            void run();
            void createTexture(const char* name, const char*filepath);

        private:
            std::vector<Texture> textures;
            
            
            const char *name;
            Device device;

    };
    
}