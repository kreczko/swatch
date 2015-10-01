/* 
 * File:   TTCInterface.cpp
 * Author: ale
 * 
 * Created on September 21, 2015, 12:20 PM
 */

#include "swatch/amc13/TTCInterface.hpp"

// AMC13 Headers
#include "amc13/AMC13.hh"

// SWATCH Headers
#include "swatch/core/MetricConditions.hpp"

namespace swatch {
namespace amc13 {


TTCInterface::TTCInterface( ::amc13::AMC13& aDriver ) :
  core::MonitorableObject( "ttc" ),
  mDriver(aDriver),
  mClockFreq( registerMetric<double>("clkFreq", core::InvRangeCondition<double>(39.9e6, 40.1e6)) ),
  mBC0Counter( registerMetric<uint32_t>("bc0Counter") ),
  BC0Errors( registerMetric<uint32_t>("bc0Errors", core::GreaterThanCondition<uint32_t>(0)) ),
  mSingleBitErrors( registerMetric<uint32_t>("ttcSingleBitErrors", core::GreaterThanCondition<uint32_t>(0)) ),
  mDoubleBitErrors( registerMetric<uint32_t>("ttcDoubleBitErrors", core::GreaterThanCondition<uint32_t>(0)) ) {
}

TTCInterface::~TTCInterface() {
}

// --------------------------------------------------------
void TTCInterface::retrieveMetricValues() {
  
  using ::amc13::AMC13;

  setMetricValue<>(mClockFreq, (double) mDriver.read(AMC13::T2,"STATUS.TTC.CLK_FREQ")*50 ); 
  setMetricValue<>(mBC0Counter, mDriver.read(AMC13::T2,"STATUS.TTC.BC0_COUNTER"));
  setMetricValue<>(BC0Errors, mDriver.read(AMC13::T2,"STATUS.TTC.BCNT_ERROR"));
  setMetricValue<>(mSingleBitErrors, mDriver.read(AMC13::T2,"STATUS.TTC.SBIT_ERROR"));
  setMetricValue<>(mDoubleBitErrors, mDriver.read(AMC13::T2,"STATUS.TTC.MBIT_ERROR"));
}

} //namespace amc13
} // namespace swatch 
