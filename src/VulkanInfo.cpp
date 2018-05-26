#include "VulkanInfo.h"

#include <iostream>

void VulkanInfo::GetLayers() {

  
  // Get number of instance layers
  uint32_t instanceLayerCount;
  vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
  
  std::vector<VkLayerProperties> layerProperties;
  layerProperties.resize(instanceLayerCount);
  vkEnumerateInstanceLayerProperties(&instanceLayerCount,
				     layerProperties.data());

  Layers.resize(instanceLayerCount);

  for (int i = 0; i < instanceLayerCount; i++) {
    
    const VkLayerProperties &prop = layerProperties[i];
    Layers[i].LayerProperties = prop;

    uint32_t layerExtensionCount;
    vkEnumerateInstanceExtensionProperties(prop.layerName,
					   &layerExtensionCount,
					   nullptr);
    
    Layers[i].ExtensionProperties.resize(layerExtensionCount);
    vkEnumerateInstanceExtensionProperties(prop.layerName,
					   &layerExtensionCount,
					   Layers[i].ExtensionProperties.data());
  }
}

void VulkanInfo::GetPhysicalDevices(const VkInstance& instance) {
  
  // Get number of devices
  uint32_t physicalDeviceCount;
  vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

  std::vector<VkPhysicalDevice> physicalDevices;
  physicalDevices.resize(physicalDeviceCount);  
  vkEnumeratePhysicalDevices(instance, &physicalDeviceCount,
  			     physicalDevices.data());

  PhysicalDevices.resize(physicalDeviceCount);

  for (int i = 0; i < physicalDeviceCount; i++) {

    const VkPhysicalDevice &physicalDevice = physicalDevices[i];
    PhysicalDevices[i].PhysicalDevice = physicalDevice;
    
    PhysicalDevices[i].ExtensionProperties.resize(Layers.size());    
    for (int j = 0; j < Layers.size(); j++) {
      uint32_t deviceExtensionCount;
      vkEnumerateDeviceExtensionProperties(physicalDevice,
  					   Layers[j].LayerProperties.layerName,
  					   &deviceExtensionCount,
  					   nullptr);
      
      PhysicalDevices[i].ExtensionProperties[j].resize(deviceExtensionCount);
      vkEnumerateDeviceExtensionProperties(physicalDevice,
  					   Layers[j].LayerProperties.layerName,
  					   &deviceExtensionCount,
  					   PhysicalDevices[i].ExtensionProperties[j].data());

    }
  }
}
