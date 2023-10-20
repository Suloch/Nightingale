
#pragma once
#include <vulkan/vulkan.h>
#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"
#include <vector>
#include "../../gameobject/gameobject.hpp"
#include "../../render/texture/texture.hpp"

namespace nge
{
    
    class Interface{
        private:
            VkDevice device;
            static ImGui_ImplVulkanH_Window g_MainWindowData;
	        VkDescriptorPool imguiPool;
	        GameObject *selectedGameObject = nullptr;
            std::string selectedTexture = "";
            std::map<std::string, Texture *> textures;
            std::vector<GameObject *> gameObjects;

            void showFileMenu();
            void showFileTree(std::map<std::string, Texture *> textures);
            void showLevelItems(std::map<std::string, GameObject *> gameObjects);
            void showConsole();
            
            
        public:
            Interface(
                VkDevice device, 
                GLFWwindow *window, 
                VkPhysicalDevice pDevice, 
                VkInstance instance, 
                VkQueue gQueue, 
                VkRenderPass renderpass,
                VkCommandPool commandPool
            );
            ~Interface();

            void showEditorInterface(std::map<std::string, Texture *> textures, std::map<std::string, GameObject *> gameObjects); 
            
    };
} // namespace nge

