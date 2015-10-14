/* 
 * File:   DAQInterface.cpp
 * Author: ale
 * 
 * Created on September 21, 2015, 12:21 PM
 */

#include "swatch/amc13/EVBInterface.hpp"

#include "swatch/amc13/Utilities.hpp"

#include "swatch/core/MonitorableObject.hpp"
#include "swatch/core/MetricConditions.hpp"

#include "amc13/AMC13.hh"

namespace swatch {
namespace amc13 {

EVBInterface::EVBInterface( ::amc13::AMC13& aDriver ) :
  dtm::EVBInterface(),
  mDriver(aDriver),
  mOverflowWarning(registerMetric<bool>("warningOverflow",core::EqualCondition<bool>(true)) ),
  mSyncLost(registerMetric<bool>("outOfSync",core::EqualCondition<bool>(true)) ),
  mAMCsTTSState(registerMetric<uint32_t>("tts",core::EqualCondition<uint32_t>(0x0),core::NotEqualCondition<uint32_t>(0x8)) ),
  mTTSState(registerMetric<uint32_t>("amcsTts",core::EqualCondition<uint32_t>(0x0),core::NotEqualCondition<uint32_t>(0x8)) ),
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
  using ::amc13::AMC13;

  const std::string prefixStat = "STATUS.";
  const std::string prefixEvb = "STATUS.EVB.";
  const std::string prefixGeneral = "STATUS.GENERAL.";
  setMetricValue<>(mOverflowWarning, (bool)mDriver.read(AMC13::T1,prefixEvb+"OVERFLOW_WARNING"));
  setMetricValue<>(mSyncLost, (bool)mDriver.read(AMC13::T1,prefixEvb+"SYNC_LOST"));
  setMetricValue<>(mAMCsTTSState, mDriver.read(AMC13::T1,prefixStat+"AMC_TTS_STATE"));
  setMetricValue<>(mTTSState, mDriver.read(AMC13::T1,prefixStat+"T1_TTS_STATE"));
  setMetricValue<>(mL1ACount, read64bCounter(mDriver, AMC13::T1,prefixGeneral+"L1A_COUNT"));
  setMetricValue<>(mRunTime, read64bCounter(mDriver, AMC13::T1,prefixGeneral+"RUN_TIME"));
  setMetricValue<>(mReadyTime, read64bCounter(mDriver, AMC13::T1,prefixGeneral+"READY_TIME"));
  setMetricValue<>(mBusyTime, read64bCounter(mDriver, AMC13::T1,prefixGeneral+"BUSY_TIME"));
  setMetricValue<>(mSyncLostTime, read64bCounter(mDriver, AMC13::T1,prefixGeneral+"SYNC_LOST_TIME"));
  setMetricValue<>(mOverflowWarningTime, read64bCounter(mDriver, AMC13::T1,prefixGeneral+"OF_WARN_TIME"));


}


} // namespace amc13
} // namespace swatch


