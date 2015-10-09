/* 
 * File:   TTCInterface.cpp
 * Author: ale
 * 
 * Created on September 21, 2015, 12:20 PM
 */

#include "swatch/dtm/TTCInterface.hpp"

// SWATCH Headers
#include "swatch/core/MetricConditions.hpp"

namespace swatch {
namespace dtm {


// --------------------------------------------------------
TTCInterface::TTCInterface() :
  core::MonitorableObject( "ttc" ),
  mClockFreq( registerMetric<double>("clkFreq", core::InvRangeCondition<double>(39.9e6, 40.1e6)) ),
  mBC0Counter( registerMetric<uint32_t>("bc0Counter") ),
  mBC0Errors( registerMetric<uint32_t>("bc0Errors", core::GreaterThanCondition<uint32_t>(0)) ),
  mSingleBitErrors( registerMetric<uint32_t>("ttcSingleBitErrors", core::GreaterThanCondition<uint32_t>(0)) ),
  mDoubleBitErrors( registerMetric<uint32_t>("ttcDoubleBitErrors", core::GreaterThanCondition<uint32_t>(0)) ) {
}

// --------------------------------------------------------
TTCInterface::~TTCInterface() {
}

} //namespace amc13
} // namespace swatch 
