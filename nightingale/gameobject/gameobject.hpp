#pragma once

#include <map>
#include <string>
#include "../physics2D/physics2D.hpp"
#include "../render/renderer/buffer/buffer.hpp"
namespace nge{

    struct Mesh2D
    {
        std::vector<std::vector<float>> vertices;

        std::vector<uint16_t> indices;
    };
    
    class GameObject{
        public:
            unsigned int id;
            
            Transform2D *transform;
            Mesh2D mesh;
            float texScale;
            float texOffsetX;
            float texOffsetY;
            int flipX;
            int flipY;
            std::map<std::string, std::string> properties;
            std::string name;
            GameObjectBuffer *buffer;
            
            void updateUniformBuffer(VkExtent2D extent, Camera2D *camera, float aspectRatio);
            
            GameObject(std::string name, int height, int width);
            ~GameObject();
            void setTexScale(float scale, float x, float y);
        
    };
} // namespace nge
