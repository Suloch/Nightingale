#pragma once

#include "gameobject/gameobject.hpp"
#include "render/renderer/render.hpp"


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
            const int MAX_FRAMES_IN_FLIGHT = 2;

            std::map<std::string, Scene> scenes;
            
            Nightingale(int height, int width, const char* name);
            ~Nightingale();

            void run();
            void createTexture(const char* name, const char*filepath);
            void loadScene(std::string name);

        private:
            std::map<std::string, std::vector<VkDescriptorSet>> dSets;
            std::vector<GameObjectBuffer *> buffers;
            std::map<std::string, Pipeline *> pipelines;
            Device *device;
            Window *window;
            PipelineLayout *pipelineLayout;
            Command *command;
            VkRenderPass renderpass;
            SyncObjects *syncObjects;

            VkDescriptorPool dPool;
            VkDescriptorSetLayout dLayout;

            int currentFrame = 0;
            const char *name;

    };
    
}