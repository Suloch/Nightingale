
#include "device.hpp"
#include "../constants.hpp"

#include<stdexcept>
#include<vector>
#include<set>
#include<iostream>
#include<GLFW/glfw3.h>
#include "swap_chain.hpp"




uint32_t ne::create_device(
    VkInstance instance,
    VkSurfaceKHR surface,
    ne::Device *device, 
    bool enableValidationLayers, 
    std::vector<const char*> validationLayers
){

    pick_physical_device(device, instance, surface);
    create_logical_device(
        device -> physical_device, 
        &device ->device,
        enableValidationLayers,
        validationLayers,
        &device ->graphics_queue,
        &device ->present_queue,
        surface
    );
    std::cout<<"device created!"<<std::endl;
    return NE_SUCCESS;
}


uint32_t ne::destroy_device(ne::Device device){
    vkDestroyDevice(device.device, nullptr);
    std::cout<<"device destroyed!";
    return NE_SUCCESS;
}


uint32_t ne::create_logical_device(
    VkPhysicalDevice physical_device, 
    VkDevice *device, 
    bool enableValidationLayers,
    std::vector<const char*> validationLayers,
    VkQueue *graphics_queue,
    VkQueue *present_queue, 
    VkSurfaceKHR surface
){  
    std::cout<<"Physical Device:"<<"picking logical device"<<physical_device<<std::endl;

    QueueFamilyIndices indices = find_queue_families(physical_device, surface);


    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphics_family.value(), indices.present_family.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }
    std::cout<<"creating device!"<<std::endl;
    
    if (vkCreateDevice(physical_device, &createInfo, nullptr, device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }
    std::cout<<"created device!"<<std::endl;
    vkGetDeviceQueue(*device, indices.graphics_family.value(), 0, graphics_queue);
    vkGetDeviceQueue(*device, indices.present_family.value(), 0, present_queue);

    return NE_SUCCESS;
}   


bool ne::is_device_suitable(VkPhysicalDevice device, VkSurfaceKHR surface){
    QueueFamilyIndices indices = ne::find_queue_families(device, surface);

    bool extensions_supported = check_device_extension_support(device);

    bool swap_chain_adequate = false;

    if(extensions_supported){
        SwapChainSupportDetails swap_chain_support = ne::query_swap_chain_support(device, surface);
        swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();
    }

    VkPhysicalDeviceFeatures supported_features;
    vkGetPhysicalDeviceFeatures(device, &supported_features);

    return indices.is_complete() && extensions_supported; // && swap_chain_adequate && supported_features.samplerAnisotropy;
}



uint32_t ne::pick_physical_device(Device *n_device, VkInstance instance, VkSurfaceKHR surface){
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

    if(device_count == 0){
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(device_count);

    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

    for(const auto& device: devices){
        if(ne::is_device_suitable(device, surface)){
            n_device -> physical_device = device;
            break;
        }
    }

    std::cout<<"Final device :"<<n_device ->physical_device<<std::endl;
    if(n_device -> physical_device == VK_NULL_HANDLE){
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    return NE_SUCCESS;
}

bool ne::check_device_extension_support(VkPhysicalDevice device){
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}
