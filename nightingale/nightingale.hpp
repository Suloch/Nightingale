#pragma once

#include "gameobject/gameobject.hpp"
#include "render/renderer/render.hpp"
#include "gameobject/camera.hpp"

#include<map>
#include<string>
#include<vector>

namespace nge{
    class Scene{
        public:
            Scene(){}
            Scene(const char* name);
            ~Scene();
            std::vector<GameObject *> gameObjects;
        private:
            const char *name;
    };

    class Nightingale{
    

        public:
            Physics2D physic2d;
            const int MAX_FRAMES_IN_FLIGHT = 2;
            Camera2D *camera;
            

            std::map<std::string, Scene> scenes;
            
            Nightingale(int height, int width, const char* name);
            ~Nightingale();

            void run();
            void createTexture(const char* name, const char*filepath);
            void createObject(std::string name);
            void loadScene(std::string name);
            void setEditorMode(bool value);
            
        private:
            std::map<std::string, Texture *> textures;
            std::map<std::string, VkDescriptorSet> dSets;
            std::vector<GameObjectBuffer *> buffers;
            std::map<std::string, Pipeline *> pipelines;
            Device *device;
            Window *window;
            PipelineLayout *pipelineLayout;
            Command *command;
            VkRenderPass renderpass;
            Interface *interface;
            VkDescriptorPool dPool;

            int currentFrame = 0;
            const char *name;
            bool editorMode;
            
            void handleCommands();

    };
    
}