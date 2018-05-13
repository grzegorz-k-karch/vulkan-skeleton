##############################################
# based on https://cmake.org/cmake-tutorial/ #
##############################################

cmake_minimum_required(VERSION 2.8)
project(VulkanSkeleton)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(LINK_FLAGS "${LINK_FLAGS} pkg-config --static --libs glfw3")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} `pkg-config --static --libs glfw3`")

set(VULKAN_SDK_DIR CACHE PATH "Path to Vulkan SDK")
set(VK_LAYER_PATH CACHE PATH "Path to Vulkan SDK")
set(GLFW_INC_DIR CACHE PATH "Path to GLFW include directory")
set(GLFW_LIB_DIR CACHE PATH "Path to GLFW library directory")

# glog - Google Logging Library - for logging
set(GLOG_DIR CACHE PATH "Path to glog directory")

if(NOT VULKAN_SDK_DIR)

endif()

# version number
set(VulkanSkeleton_VERSION_MAJOR 0)
set(VulkanSkeleton_VERSION_MINOR 1)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/VulkanSkeletonConfig.h.in"
  "${PROJECT_BINARY_DIR}/VulkanSkeletonConfig.h"
  )

# add the binary tree to the search path for include files
# so that we will find VulkanSkeletonConfig.h
include_directories(${PROJECT_BINARY_DIR})
include_directories(${VULKAN_SDK_DIR}/include) # Vulkan
include_directories(${GLFW_INC_DIR}) # GLFW
include_directories(${GLOG_DIR}/src) # glog
include_directories(${GLOG_DIR}/build)
  
link_directories(${VULKAN_SDK_DIR}/lib)
link_directories(${GLFW_LIB_DIR})
link_directories(${GLOG_DIR}/build)

add_executable(VulkanSkeleton main.cpp
  ParseConfigXML.cpp)

target_link_libraries(VulkanSkeleton
  vulkan
  glfw3
  dl
  pthread
  Xxf86vm
  X11
  Xcursor
  Xrandr
  Xi
  Xinerama
  glog
  xcb)

