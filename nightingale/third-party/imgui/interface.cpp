

#include "interface.hpp"
#include <stdexcept>
#include <GLFW/glfw3.h>
#include "../../render/utils.hpp"

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

}