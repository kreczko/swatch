/* 
 * File:   ReadoutInterface.cpp
 * Author: Tom Williams
 * Created August 2015
 */

#include "swatch/processor/ReadoutInterface.hpp"


#include "swatch/core/MetricConditions.hpp"


namespace swatch {
namespace processor {
  
ReadoutInterface::ReadoutInterface() : 
  core::MonitorableObject("readout")
{
}


ReadoutInterface::~ReadoutInterface()
{
}


const std::vector<std::string>
ReadoutInterface::defaultMetrics = {};


} // end ns core
} // end ns swatch
