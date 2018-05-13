#ifndef PARSE_CONFIG_XML_H
#define PARSE_CONFIG_XML_H

#include "VulkanSkeletonConfig.h"

#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

void ReadConfigXML(std::istream& is, VulkanSkeletonConfig& config);

#endif//PARSE_CONFIG_XML_H
