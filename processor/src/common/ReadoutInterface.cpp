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
const std::vector<std::string> ReadoutInterface::kDefaultMetrics = {"tts"};

  
ReadoutInterface::ReadoutInterface() : 
  core::MonitorableObject("readout"),
  mMetricTTS( registerMetric<uint32_t>("tts") ),
  mMetricAMCCoreReady( registerMetric<bool>("amcCoreReady") )       
{

  // Error if in OOS, warning if not Ready
  setConditions(mMetricTTS,
      core::EqualCondition<uint32_t>(0x2),
      core::NotEqualCondition<uint32_t>(0x8));

  // Error if AMCCore is not ready
  setErrorCondition(mMetricAMCCoreReady, core::EqualCondition<bool>(false));
}


ReadoutInterface::~ReadoutInterface()
{
}




} // end ns core
} // end ns swatch
