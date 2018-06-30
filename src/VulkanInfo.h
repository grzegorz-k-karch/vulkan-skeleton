#ifndef VULKAN_INFO_H_
#define VULKAN_INFO_H_

// Vulkan
#include "vulkan/vulkan_core.h"
#include "vulkan/vulkan.h"
// Google logging library
#include "glog/logging.h"

// std
#include <vector>

struct VulkanLayer {
  VkLayerProperties LayerProperties;
  std::vector<VkExtensionProperties> ExtensionProperties;
};

struct VulkanPhysicalDevice {
  VkPhysicalDevice PhysicalDevice;
  std::vector<std::vector<VkExtensionProperties>> ExtensionProperties;
};

struct VulkanInfo {

  std::vector<VulkanLayer> Layers;
  void GetLayers();
  
  std::vector<VulkanPhysicalDevice> PhysicalDevices;
  void GetPhysicalDevices(const VkInstance& instance);
  
  std::vector<VkQueueFamilyProperties> QueueFamilyProperties;
  std::vector<VkPhysicalDeviceMemoryProperties> PhysicalDeviceMemoryProperties;
  std::vector<VkPhysicalDeviceProperties> PhysicalDeviceProperties;
  std::vector<VkSurfaceFormatKHR> SurfaceFormats;  
};

struct VulkanInstance {

  ~VulkanInstance() {
    vkDestroyInstance(Instance);
  }
  
  VkInstance Instance;
  void CreateInstance(const std::vector<const char*>& layerNames,
			  const std::vector<const char*>& extensionNames) {

    VkInstanceCreateInfo instanceInfo = {};
    instanceInfo.ppEnabledLayerNames = layerNames.data();
    instanceInfo.ppEnabledExtensionNames = extensionNames.data();

    if (vkCreateInstance(&instanceInfo, nullptr, &Instance) != VK_SUCCESS) {
      LOG(FATAL) << "Could not create Vulkan instance. Exiting";
    }
  }
};


#endif // VULKAN_INFO_H_
