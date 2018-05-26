#ifndef VULKAN_INFO_H_
#define VULKAN_INFO_H_

#include "vulkan/vulkan_core.h"
#include "vulkan/vulkan.h"

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


#endif // VULKAN_INFO_H_
