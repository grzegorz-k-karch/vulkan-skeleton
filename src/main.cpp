#define VK_USE_PLATFORM_XCB_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
// Google logging library
#include "glog/logging.h"

// std
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

// project
#include "VulkanSkeletonConfig.h"
#include "ParseConfigXML.h"

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

  // xcb_map_window(conn, window);

  // // test  
  // xcb_flush(conn);
  
  // xcb_generic_event_t *e;
  // int done = 0;
  // xcb_rectangle_t r = { 20, 20, 60, 60 };
  
  // /* event loop */
  // while (!done && (e = xcb_wait_for_event(conn))) {
  //   switch (e->response_type & ~0x80) {
  //   case XCB_EXPOSE:    /* draw or redraw the window */
  //     xcb_poly_fill_rectangle(conn, parent, window,  1, &r);
  //     xcb_flush(conn);
  //     break;
  //   case XCB_KEY_PRESS:  /* exit on key press */
  //     done = 1;
  //     break;
  //   }
  //   free(e);
  // }
  // /* close connection to server */
  // xcb_disconnect(conn);
  // //~test

  // return window;
}

int main(int argc,  char** argv) {

  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);

  if (argc != 2) {
    LOG(FATAL) << "Usage: " << argv[0] << " <xml  config file>. Exiting";
  }

  // Initialize GLFW
  if (!glfwInit()) {
    LOG(FATAL) << "Could not initialize GLFW. Exiting";
  }
  if (!glfwVulkanSupported()) {
    LOG(FATAL) << "Vulkan is not suported. Exiting";
  }

  VulkanInfo vulkanInfo;
  VkResult result = VK_SUCCESS;
  //==========================================================================  
  // 1. Enumerate Instance Layer properties
  uint32_t instanceLayerCount;
  
  // Get number of instance layers
  vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
  vulkanInfo.InstanceLayerProperties.resize(instanceLayerCount);
  vkEnumerateInstanceLayerProperties(&instanceLayerCount,
				     vulkanInfo.InstanceLayerProperties.data());

  vulkanInfo.InstanceExtensionProperties.resize(instanceLayerCount);

  for (int i = 0; i < instanceLayerCount; i++) {
    const VkLayerProperties &prop = vulkanInfo.InstanceLayerProperties[i];

    uint32_t instanceExtensionCount;
    vkEnumerateInstanceExtensionProperties(prop.layerName,
					   &instanceExtensionCount,
					   nullptr);
    vulkanInfo.InstanceExtensionProperties[i].resize(instanceExtensionCount);
    vkEnumerateInstanceExtensionProperties(prop.layerName,
					   &instanceExtensionCount,
  					   vulkanInfo.InstanceExtensionProperties[i].data());
  }

  //==========================================================================
  // 2. Instance Creation
  VkInstanceCreateInfo instanceInfo = {};

  // Specify layer names that need to be enabled on instance
  std::vector<const char*> layers;
  layers.push_back("VK_LAYER_LUNARG_standard_validation");
  layers.push_back("VK_LAYER_LUNARG_object_tracker");
  instanceInfo.ppEnabledLayerNames = layers.data();

  // Specify extensions that need to be enabled on instance
  std::vector<const char*> extensions;
  extensions.push_back("VK_KHR_SURFACE_EXTENSION_NAME");
  extensions.push_back("VK_KHR_WIN32_SURFACE_EXTENSION_NAME");
  instanceInfo.ppEnabledExtensionNames = extensions.data();

  // Vulkan instance object
  VkInstance instance;
  if (vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS) {
    LOG(FATAL) << "Could not create Vulkan instance. Exiting";
  }

  //==========================================================================
  // 3. Enumerate Physical Devices
  uint32_t physicalDeviceCount;
  
  // Get number of devices
  vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
  vulkanInfo.PhysicalDevices.resize(physicalDeviceCount);
  vkEnumeratePhysicalDevices(instance, &physicalDeviceCount,
  			     vulkanInfo.PhysicalDevices.data());

  vulkanInfo.DeviceExtensionProperties.resize(physicalDeviceCount);

  for (int i = 0; i < physicalDeviceCount; i++) {

    VkPhysicalDevice &physicalDevice = vulkanInfo.PhysicalDevices[i];

    vulkanInfo.DeviceExtensionProperties[i].resize(layers.size());
    for (int j = 0; j < layers.size(); j++) {
      uint32_t deviceExtensionCount;
      vkEnumerateDeviceExtensionProperties(physicalDevice,
					   layers[j],
					   &deviceExtensionCount,
					   nullptr);
      vulkanInfo.DeviceExtensionProperties[i][j].resize(deviceExtensionCount);
      vkEnumerateDeviceExtensionProperties(physicalDevice,
					   layers[j],
					   &deviceExtensionCount,
					   vulkanInfo.DeviceExtensionProperties[i][j].data());
    }
  }
  

  //==========================================================================
  // 4. Create Device  
  // TODO: here just taking the first device
  const int physicalDeviceId = 0;
 
  uint32_t queueFamilyCount;
  vkGetPhysicalDeviceQueueFamilyProperties(vulkanInfo.PhysicalDevices[physicalDeviceId],
					   &queueFamilyCount,
					   nullptr);
  vulkanInfo.QueueFamilyProperties.resize(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(vulkanInfo.PhysicalDevices[physicalDeviceId],
					   &queueFamilyCount,
  					   vulkanInfo.QueueFamilyProperties.data());

  vulkanInfo.PhysicalDeviceMemoryProperties.resize(physicalDeviceCount);
  vkGetPhysicalDeviceMemoryProperties(vulkanInfo.PhysicalDevices[physicalDeviceId],
				      &vulkanInfo.PhysicalDeviceMemoryProperties[physicalDeviceId]);

  vulkanInfo.PhysicalDeviceProperties.resize(physicalDeviceCount);
  vkGetPhysicalDeviceProperties(vulkanInfo.PhysicalDevices[physicalDeviceId],
				&vulkanInfo.PhysicalDeviceProperties[physicalDeviceId]);
  
  // Create the logical device object from physical device
  VkDeviceCreateInfo deviceInfo = {};
  VkDevice device;
  if (vkCreateDevice(vulkanInfo.PhysicalDevices[physicalDeviceId],
		     &deviceInfo,
		     nullptr,
		     &device) != VK_SUCCESS) {
    LOG(FATAL) << "Could not create logical device. Exiting.";
  }

  //==========================================================================
  // 5. Presentation Initialization
  xcb_window_t window;
  xcb_connection_t *connection;
  
  CreateWindow(&window, &connection);

  VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
						 nullptr,
						 0,
						 connection,
						 window};
  VkSurfaceKHR surface;
  result = vkCreateXcbSurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface);
  if (result != VK_SUCCESS) {
    LOG(FATAL) << "Could not create surface (Error: " << result << "). Exiting.";
  }

  // vkGetPhysicalDeviceSurfaceSupportKHR(device, queueIndex, surface,
  //                                      &isPresentationSupported);
  // if (isPresentationSupported) {
  //   graphicsQueueFamilyIndex = queueIndex;
  //   break;
  // }
  // vkGetDeviceQueue(device, graphicsQueueFamilyIndex, 0, &queue);

  // uint32_t formatCount;
  // vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
  // VkSurfaceFormatKHR *surfaceFormats = alloc(formatCount*VkSurfaceFormatKHR);
  // vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, surfaceFormats);

  // vkBeginCommandBuffer(cmd, &cmdBufferInfo);
  // vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &surfaceCapabilities);
  // vkGetPhysicalDeviceSurfacePresentModesKHR(device,  surface, &presentModeCount, nullptr);
  // VkPresentModeKHR presentModes[presentModeCount];
  // vkGetPhysicalDeviceSurfacePresentModesKHR(device,  surface, &presentModeCount, presentModes);

  // VkSwapchainCreateInfoKHR swapChainInfo = {};
  // vkCreateSwapchainKHR(device, &swapChainInfo, nullptr, &swapChain);

  // vkGetSwapchainImagesKHR(device, swapChain, &swapChainImageCount, nullptr);
  // VkImage swapChainImages[swapChainImageCount];
  // vkGetSwapchainImagesKHR(device, swapChain, &swapChainImageCount, swapChainImages);

  // for (swapChainImages) {
  // SetImageLayout();
  // VkImageMemoryBarrier imageMemoryBarrier = {};
  // vkCmdPipelineBarrier(cmd, srcStages, destStages, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
  // vkCreateImageView(device, &colorImageView,  nullptr, &scBuffer.view);
  // buffers.push_back(scBuffer);
  // }
  
  return 0;
}
  // VulkanSkeletonConfig skeletonConfig;

  // std::ifstream xmlfile(argv[1], std::ifstream::in);   
  // ReadConfigXML(xmlfile, skeletonConfig);
  // xmlfile.close();
