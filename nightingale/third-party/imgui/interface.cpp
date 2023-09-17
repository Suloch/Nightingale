

#include "interface.hpp"
#include <stdexcept>
#include <GLFW/glfw3.h>
#include "../../render/utils.hpp"
#include "../icons/IconsForkAwesome.h"
#include "../../levelbuilder/commandqueue.hpp"
#include "../logger/logger.hpp"
#include <filesystem>

struct DisplayPath{
	std::string name;
	std::string ext;
	bool isDirectory;

};

std::vector<DisplayPath> get_items_in_current_directory(std::string path, std::vector<std::string> allowedExtensions){
	std::vector<DisplayPath> items;
    for (const auto & entry : std::filesystem::directory_iterator(path)){
		DisplayPath d;
		d.name = entry.path().filename();
		d.ext = entry.path().extension();
		d.isDirectory = entry.is_directory();

		for(auto ext: allowedExtensions){
			if(d.ext == ext || d.isDirectory ){
				items.push_back(d);
				break;
			}
		}
	}
    return items;
}

nge::Interface::Interface(
	VkDevice device, 
	GLFWwindow *window, 
	VkPhysicalDevice pDevice, 
	VkInstance instance, 
	VkQueue gQueue, 
	VkRenderPass renderpass,
	VkCommandPool commandPool
	){
	this->device = device;
    VkDescriptorPoolSize pool_sizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000;
	pool_info.poolSizeCount = std::size(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;

	if(vkCreateDescriptorPool(device, &pool_info, nullptr, &this->imguiPool) != VK_SUCCESS){
        throw std::runtime_error("cannot create imgui pool");
    }



	//this initializes the core structures of imgui
	ImGui::CreateContext();

	//this initializes imgui for GLFW
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	
	io.Fonts->AddFontDefault();
	float baseFontSize = 14.0f;
	float iconFontSize = baseFontSize * 2.0f / 3.0f;
	static const ImWchar icons_ranges[] = {ICON_MIN_FK, ICON_MAX_16_FK, 0};
	
	ImFontConfig icons_config;
	icons_config.MergeMode = true; 
	icons_config.PixelSnapH = true; 
	icons_config.GlyphMinAdvanceX = iconFontSize;
	io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FK, iconFontSize, &icons_config, icons_ranges );

    ImGui_ImplGlfw_InitForVulkan(window, true);
    


	//this initializes imgui for Vulkan
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = instance;
	init_info.PhysicalDevice = pDevice;
	init_info.Device = device;
	init_info.Queue = gQueue;
	init_info.DescriptorPool = imguiPool;
	init_info.MinImageCount = 3;
	init_info.ImageCount = 3;
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

	ImGui_ImplVulkan_Init(&init_info, renderpass);

	// Upload Fonts
    {
        VkCommandBuffer cmd = beginSingleTimeCommands(commandPool, device);
		ImGui_ImplVulkan_CreateFontsTexture(cmd);
        endSingleTimeCommands(commandPool, cmd, device, gQueue);
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

	



}

nge::Interface::~Interface(){
    vkDestroyDescriptorPool(device, this->imguiPool, nullptr);
    ImGui_ImplVulkan_Shutdown();
	level::CommandQueue::getInstance().pushCommand(level::STOP_EXECUTE, {""});
}


void nge::Interface::showFileMenu(){
	static bool enableSpriteSheetEditor = false;

	if(ImGui::BeginMainMenuBar()){
		ImGui::SetWindowFontScale(1.5);

		if(ImGui::BeginMenu("File")){
			ImGui::MenuItem("New");
			ImGui::MenuItem("Open");
			ImGui::MenuItem("Save");
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Tools")){
			ImGui::MenuItem("Sprite Sheet Editor", NULL, &enableSpriteSheetEditor);
			
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if(enableSpriteSheetEditor){
		ImGui::SetNextWindowSize(ImVec2(700, 700));
		ImGui::Begin("Sprite Editor", &enableSpriteSheetEditor);
		if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Close Editor"))
                enableSpriteSheetEditor = false;
            ImGui::EndPopup();
        }
		ImGui::End();
	}
}

void nge::Interface::showFileTree(std::map<std::string, Texture *> textures){

	ImGuiWindowFlags windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoCollapse;

	ImGui::SetNextWindowPos(ImVec2(0.0f, 440.0f));
	ImGui::SetNextWindowSize(ImVec2(200.0f, 600.0f));
	ImGui::Begin("Files", NULL, windowFlags);
	ImGui::SetWindowFontScale(1.5);

		if(ImGui::Button(ICON_FK_PLUS_SQUARE " Add new texture")){
			ImGui::OpenPopup("NewTexture");
		}
		static std::string selected = "";
		//list existing files
		for(auto tex: textures){
			const bool is_selected = (selected == tex.first);
			if (ImGui::Selectable((ICON_FK_FILE_IMAGE_O + tex.first).c_str(), is_selected))
				selected = tex.first;
		}
		// if(ImGui::BeginListBox(" ")){
		// }
		// ImGui::EndListBox();
		// start pop up for selecting files
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if(ImGui::BeginPopupModal("NewTexture", NULL, ImGuiWindowFlags_AlwaysAutoResize)){
			static char buf[32] = "new_tex";
			ImGui::InputText(" ", buf, IM_ARRAYSIZE(buf));

			if(ImGui::Button(ICON_FK_FILE_IMAGE_O "Browse file")){
				ImGui::OpenPopup("Browse Files");
			}
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			
			static std::string path = "./";
			static std::string filename = "";
			static bool directory = false;
			std::string label = "Browse Files";
			// if(path != "./"){
			// 	label = filename;
			// }
			if(ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)){
				const bool is_selected = (filename == "..");
				if (ImGui::Selectable(ICON_FK_FOLDER_O  "..", is_selected, ImGuiSelectableFlags_DontClosePopups)){
					directory = true;
					filename = "..";
				}
				for(auto item: get_items_in_current_directory(path, {".png"})){
					const bool is_selected = (filename == item.name);
					if(item.isDirectory){
						if (ImGui::Selectable((ICON_FK_FOLDER_O + item.name).c_str(), is_selected, ImGuiSelectableFlags_DontClosePopups)){
							directory = true;
							filename = item.name;
						}
					}else{
						if (ImGui::Selectable( (ICON_FK_FILE_IMAGE_O + item.name).c_str(), is_selected, ImGuiSelectableFlags_DontClosePopups)){
							directory = false;
							filename = item.name;
						}
					}
				}	
				if (ImGui::Button("OK", ImVec2(120, 0))) {

					if(directory){
						path = path + filename + "/";
					}else{
						path = path + filename;
				 		ImGui::CloseCurrentPopup(); 
					}
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
				ImGui::EndPopup();
			}

			if (ImGui::Button("OK", ImVec2(120, 0))) {
				level::CommandQueue::getInstance().pushCommand(level::CREATE_TEXTURE, {buf, path});
				path = "./";
				 ImGui::CloseCurrentPopup(); 
			}
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}
	
		
	ImGui::End();
}


void nge::Interface::showLevelItems(std::vector<GameObject *> gameObjects){

	ImGuiWindowFlags windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoCollapse;

	ImGui::SetNextWindowPos(ImVec2(0.0f, 30.0f));
	ImGui::SetNextWindowSize(ImVec2(200.0f, 400.0f));

	ImGui::SetNextItemWidth(ImGui::GetFontSize() * 20);

	ImGui::Begin("Level", NULL, windowFlags);
	ImGui::SetWindowFontScale(1.5);


	if(ImGui::Button("Add")){
		// objectList.push_back("Object"+ std::to_string(objectList.size()));
		level::CommandQueue::getInstance().pushCommand(level::CREATE_OBJECT, {"Object"});
	}
	static GameObject *selected = nullptr;
	for(auto object: gameObjects){
		const bool is_selected = (selected != nullptr && selected->id == object->id);

		if (ImGui::Selectable((ICON_FK_WRENCH + object->name).c_str(), is_selected))
			selected = object;
	}
	ImGui::End();
}

void nge::Interface::showConsole(){}

void nge::Interface::showEditorInterface(std::map<std::string, Texture *> textures, std::vector<GameObject *> gameObjects){
		showFileMenu();
		showFileTree(textures);		
		showConsole();
		showLevelItems(gameObjects);		

}