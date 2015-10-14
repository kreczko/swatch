/* 
 * File:   DAQInterface.cpp
 * Author: ale
 * 
 * Created on September 21, 2015, 12:21 PM
 */

#include "swatch/amc13/EVBInterface.hpp"

#include "amc13/AMC13.hh"
#include "swatch/core/MonitorableObject.hpp"
#include "swatch/core/MetricConditions.hpp"

namespace swatch {
namespace amc13 {

EVBInterface::EVBInterface( ::amc13::AMC13& aDriver ) :
  dtm::EVBInterface(),
  mDriver(aDriver),
  mOverflowWarning(registerMetric<bool>("warningOverflow",core::EqualCondition<bool>(false)) ),
  mSyncLost(registerMetric<bool>("outOfSync",core::EqualCondition<bool>(false)) ),
  mAMCsTTSState(registerMetric<uint32_t>("amcsTTS") ),
  mTTSState(registerMetric<uint32_t>("tts") ),
  mL1ACount(registerMetric<uint64_t>("l1aCount")),
  mRunTime(registerMetric<uint64_t>("runTime")),
  mReadyTime(registerMetric<uint64_t>("readyTime")),
  mBusyTime(registerMetric<uint64_t>("busyTime")),
  mSyncLostTime(registerMetric<uint64_t>("oosTime")),
  mOverflowWarningTime(registerMetric<uint64_t>("warnTime")) {
}

EVBInterface::~EVBInterface() {
}

void 
EVBInterface::retrieveMetricValues() {

}


} // namespace amc13
} // namespace swatch


