#define VK_USE_PLATFORM_XCB_KHR
#include "vulkan/vulkan_core.h"
#include "vulkan/vulkan.h"
// Google logging library
#include "glog/logging.h"

// std
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

// project
#include "VulkanInfo.h"

// xcb
#include <xcb/xcb.h>
#include <xcb/xproto.h>

// loosely base on https://en.wikipedia.org/wiki/XCB
xcb_window_t CreateWindow(xcb_window_t *window, xcb_connection_t **conn) {

  *conn = xcb_connect(NULL,NULL);;
  if (xcb_connection_has_error(*conn)) {
    LOG(FATAL) << "XCB connection failed. Exiting";
  }
  *window = xcb_generate_id(*conn);

  xcb_screen_t *s = xcb_setup_roots_iterator(xcb_get_setup(*conn)).data;
  
  uint8_t depth = XCB_COPY_FROM_PARENT;
  xcb_window_t parent;
  int16_t x = 100;
  int16_t y = 100;
  uint16_t width = 640;
  uint16_t height = 480;
  uint16_t border_width = 0;
  uint16_t _class = XCB_WINDOW_CLASS_INPUT_OUTPUT;
  xcb_visualid_t visual = XCB_COPY_FROM_PARENT;
  uint32_t value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  const uint32_t value_list[2] = {s->white_pixel,
				  XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS};
  
  xcb_void_cookie_t window_cookie = xcb_create_window(*conn, depth, *window, parent, x, y, width, height, border_width,
						      _class, visual, value_mask, value_list);

}

int main(int argc,  char** argv) {

  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);

  VulkanInfo vulkanInfo;
  VulkanInstance vulkanInstance;
  VkResult result = VK_SUCCESS;
  
  //==========================================================================  
  // 1. Get Instance Layer properties
  vulkanInfo.GetLayers();

  //==========================================================================
  // 2. Instance Creation
  vulkanInstance.CreateInstance();
  
  //==========================================================================
  // 3. Get Physical Devices
  vulkanInfo.GetPhysicalDevices(vulkanInstance.Instance);  

  // //==========================================================================
  // // 4. Create Device  
  // // TODO: here just taking the first device
  // const int physicalDeviceId = 0;
  // VkPhysicalDevice &physicalDevice = vulkanInfo.PhysicalDevices[physicalDeviceId];
  
  // uint32_t queueFamilyCount;
  // vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,
  // 					   &queueFamilyCount,
  // 					   nullptr);
  // vulkanInfo.QueueFamilyProperties.resize(queueFamilyCount);
  // vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,
  // 					   &queueFamilyCount,
  // 					   vulkanInfo.QueueFamilyProperties.data());

  // vulkanInfo.PhysicalDeviceMemoryProperties.resize(physicalDeviceCount);
  // vkGetPhysicalDeviceMemoryProperties(physicalDevice,
  // 				      &vulkanInfo.PhysicalDeviceMemoryProperties[physicalDeviceId]);

  // vulkanInfo.PhysicalDeviceProperties.resize(physicalDeviceCount);
  // vkGetPhysicalDeviceProperties(physicalDevice,
  // 				&vulkanInfo.PhysicalDeviceProperties[physicalDeviceId]);
  
  // // Create the logical device object from physical device
  // VkDeviceCreateInfo deviceInfo = {};
  // VkDevice device;
  // if (vkCreateDevice(physicalDevice,
  // 		     &deviceInfo,
  // 		     nullptr,
  // 		     &device) != VK_SUCCESS) {
  //   LOG(FATAL) << "Could not create logical device. Exiting.";
  // }

  // //==========================================================================
  // // 5. Presentation Initialization
  // xcb_window_t window;
  // xcb_connection_t *connection;
  
  // CreateWindow(&window, &connection);

  // VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
  // 						 nullptr,
  // 						 0,
  // 						 connection,
  // 						 window};
  // VkSurfaceKHR surface;
  // result = vkCreateXcbSurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface);
  // if (result != VK_SUCCESS) {
  //   LOG(FATAL) << "Could not create surface (Error: " << result << "). Exiting.";
  // }

  // int graphicsQueueFamilyIndex;
  // for (int queueIndex = 0; queueIndex < 1; queueIndex++) {
  //   VkBool32 isPresentationSupported = false;
  //   vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice,
  // 					 queueIndex,
  // 					 surface,
  // 					 &isPresentationSupported);
  //   if (isPresentationSupported) {
  //     graphicsQueueFamilyIndex = queueIndex;
  //     break;
  //   }
  // }
  // VkQueue queue;
  // vkGetDeviceQueue(device, graphicsQueueFamilyIndex, 0, &queue);

  // uint32_t formatCount;
  // vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,
  // 				       surface,
  // 				       &formatCount,
  // 				       nullptr);
  // vulkanInfo.SurfaceFormats.resize(formatCount);
  // vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,
  // 				       surface,
  // 				       &formatCount,
  // 				       vulkanInfo.SurfaceFormats.data());

  // // vkBeginCommandBuffer(cmd, &cmdBufferInfo);
  // // vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &surfaceCapabilities);
  // // vkGetPhysicalDeviceSurfacePresentModesKHR(device,  surface, &presentModeCount, nullptr);
  // // VkPresentModeKHR presentModes[presentModeCount];
  // // vkGetPhysicalDeviceSurfacePresentModesKHR(device,  surface, &presentModeCount, presentModes);

  // // VkSwapchainCreateInfoKHR swapChainInfo = {};
  // // vkCreateSwapchainKHR(device, &swapChainInfo, nullptr, &swapChain);

  // // vkGetSwapchainImagesKHR(device, swapChain, &swapChainImageCount, nullptr);
  // // VkImage swapChainImages[swapChainImageCount];
  // // vkGetSwapchainImagesKHR(device, swapChain, &swapChainImageCount, swapChainImages);

  // // for (swapChainImages) {
  // // SetImageLayout();
  // // VkImageMemoryBarrier imageMemoryBarrier = {};
  // // vkCmdPipelineBarrier(cmd, srcStages, destStages, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
  // // vkCreateImageView(device, &colorImageView,  nullptr, &scBuffer.view);
  // // buffers.push_back(scBuffer);
  // // }
  
  return 0;
}
  // VulkanSkeletonConfig skeletonConfig;

  // std::ifstream xmlfile(argv[1], std::ifstream::in);   
  // ReadConfigXML(xmlfile, skeletonConfig);
  // xmlfile.close();
