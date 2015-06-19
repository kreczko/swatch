/* 
 * File:   TTCInterface.cpp
 * Author: Tom Williams
 * 
 * Created May 2015
 */

#include "swatch/processor/TTCInterface.hpp"


#include "swatch/core/MetricConditions.hpp"


namespace swatch {
namespace processor {

TTCInterface::TTCInterface():
  core::MonitorableObject( "ttc" ),
  metricBunchCounter_( registerMetric<uint32_t>("bunchCounter") ),
  metricEventCounter_( registerMetric<uint32_t>("eventCounter") ),
  metricOrbitCounter_( registerMetric<uint32_t>("orbitCounter") ),
  metricIsClock40Locked_( registerMetric<bool>("isClock40Locked", core::EqualCondition<bool>(false)) ),
  metricHasClock40Stopped_( registerMetric<bool>("hasClock40Stopped", core::EqualCondition<bool>(true)) ),
  metricIsBC0Locked_( registerMetric<bool>("isBC0Locked", core::EqualCondition<bool>(false)) ),
  metricSingleBitErrors_( registerMetric<uint32_t>("singleBitErrors", core::GreaterThanCondition<uint32_t>(0)) ),
  metricDoubleBitErrors_( registerMetric<uint32_t>("doubleBitErrors", core::GreaterThanCondition<uint32_t>(0)) )
{
}


std::vector<std::string>
TTCInterface::getDefaultMetrics() {
  std::vector<std::string> metrics;
  metrics.push_back("bunchCounter");
  metrics.push_back("eventCounter");
  metrics.push_back("orbitCounter");

  metrics.push_back("isClock40Locked");
  metrics.push_back("hasClock40Stopped");
  metrics.push_back("isBC0Locked");
  metrics.push_back("singleBitErrors");
  metrics.push_back("doubleBitErrors");
  
  return metrics;
}


} // end ns core
} // end ns swatch
