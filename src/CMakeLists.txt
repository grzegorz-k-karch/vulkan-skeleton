##############################################
# based on https://cmake.org/cmake-tutorial/ #
##############################################

cmake_minimum_required(VERSION 3.5)

# project name
project(VulkanSkeleton)

# version number
set(VulkanSkeleton_VERSION_MAJOR 0)
set(VulkanSkeleton_VERSION_MINOR 1)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/VulkanSkeletonConfig.h.in"
  "${PROJECT_BINARY_DIR}/VulkanSkeletonConfig.h"
  )

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(VULKAN_SDK CACHE PATH "Path to Vulkan SDK")
set(VK_LAYER_PATH "${VULKAN_SDK}/etc/explicit_layer.d")

# glog - Google Logging Library - for logging
set(GLOG_DIR CACHE PATH "Path to glog directory")

# add the binary tree to the search path for include files
# so that we will find VulkanSkeletonConfig.h
include_directories(${PROJECT_BINARY_DIR})
include_directories(${VULKAN_SDK}/include) # Vulkan
include_directories(${GLOG_DIR}/src) # glog
  
link_directories(${VULKAN_SDK}/lib)
link_directories(${GLOG_DIR}/build)

add_executable(VulkanSkeleton main.cpp
  VulkanInfo.cpp)

target_link_libraries(VulkanSkeleton
  vulkan
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

