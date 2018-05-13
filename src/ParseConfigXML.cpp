#include "ParseConfigXML.h"
#include "VulkanSkeletonConfig.h"

#include <iostream>

void ReadConfigXML(std::istream& is, VulkanSkeletonConfig& config) {
  
  using boost::property_tree::ptree;
  ptree pt;
  read_xml(is, pt);

  for (ptree::value_type const& v : pt.get_child("vulkanConfig") ) {
    if( v.first == "layer" ) {
      std::string layer_name = v.second.get("<xmlattr>.name", "");
      std::cout << layer_name << std::endl;
      std::string extension = v.second.get<std::string>("extension");
      std::cout << extension << std::endl;
    }
  }
}
