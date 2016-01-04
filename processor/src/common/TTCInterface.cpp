/* 
 * File:   TTCInterface.cpp
 * Author: Tom Williams
 * Created May 2015
 */

#include "swatch/processor/TTCInterface.hpp"

// SWATCH Headers
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


const std::vector<std::string>
TTCInterface::kDefaultMetrics = {
  "bunchCounter", 
  "eventCounter",
  "orbitCounter",

  "isClock40Locked",
  "hasClock40Stopped",
  "isBC0Locked",
  "singleBitErrors",
  "doubleBitErrors",
};


} // end ns core
} // end ns swatch
