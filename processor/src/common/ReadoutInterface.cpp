/* 
 * File:   ReadoutInterface.cpp
 * Author: Tom Williams
 * Created August 2015
 */

#include "swatch/processor/ReadoutInterface.hpp"


#include "swatch/core/MetricConditions.hpp"


namespace swatch {
namespace processor {

// Static Members Initialization
const std::vector<std::string> ReadoutInterface::defaultMetrics = {"tts"};

  
ReadoutInterface::ReadoutInterface() : 
  core::MonitorableObject("readout"),
  mMetricTTS( registerMetric<uint32_t>("tts", core::EqualCondition<uint32_t>(0), core::NotEqualCondition<uint32_t>(0x8)) )        

{
}


ReadoutInterface::~ReadoutInterface()
{
}




} // end ns core
} // end ns swatch
