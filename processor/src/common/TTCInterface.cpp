/* 
 * File:   TTCInterface.cpp
 * Author: Tom Williams
 * 
 * Created May 2015
 */

#include "swatch/processor/TTCInterface.hpp"


namespace swatch {
namespace processor {

TTCInterface::TTCInterface():
core::MonitorableObject( "ttc" ),
metricIsClock40Locked_( registerMetric<bool>("isClock40Locked", true, true) ),
metricHasClock40Stopped_( registerMetric<bool>("hasClock40Stopped", false, false) ),
metricIsBC0Locked_( registerMetric<bool>("isBC0Locked", true, true) ),
metricSingleBitErrors_( registerMetric<uint32_t>("singleBitErrors", 0, 0) ),
metricDoubleBitErrors_( registerMetric<uint32_t>("doubleBitErrors", 0, 0) )
{
}


std::vector<std::string>
TTCInterface::getDefaultMetrics() {
  std::vector<std::string> metrics;
  metrics.push_back("isClock40Locked");
  metrics.push_back("hasClock40Stopped");
  metrics.push_back("isBC0Locked");
  metrics.push_back("singleBitErrors");
  metrics.push_back("doubleBitErrors");
  
  return metrics;
}


} // end ns core
} // end ns swatch
