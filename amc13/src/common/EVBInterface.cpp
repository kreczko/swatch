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
#include "swatch/core/TTSUtils.hpp"

namespace swatch {
namespace amc13 {

EVBInterface::EVBInterface( ::amc13::AMC13& aDriver ) :
  dtm::EVBInterface(),
  mDriver(aDriver),
  mOverflowWarning(registerMetric<bool>("warningOverflow") ),
  mSyncLost(registerMetric<bool>("outOfSync") ),
  mAMCsTTSState(registerMetric<std::string>("amcsTts") ),
  mTTSState(registerMetric<std::string>("tts") ),
  mL1ACount(registerMetric<uint64_t>("l1aCount")),
  mRunTime(registerMetric<uint64_t>("runTime")),
  mReadyTime(registerMetric<uint64_t>("readyTime")),
  mBusyTime(registerMetric<uint64_t>("busyTime")),
  mSyncLostTime(registerMetric<uint64_t>("oosTime")),
  mOverflowWarningTime(registerMetric<uint64_t>("warnTime")) {
  
  // Assign Error and Warning conditions
  setErrorCondition(mOverflowWarning,core::EqualCondition<bool>(true));
  setErrorCondition(mSyncLost,core::EqualCondition<bool>(true));

  setConditions(mAMCsTTSState,
      core::EqualCondition<std::string>(core::tts::kErrorStr),
      core::NotEqualCondition<std::string>(core::tts::kReadyStr)
      );
  
  setConditions(mTTSState,
      core::EqualCondition<std::string>(core::tts::kErrorStr),
      core::NotEqualCondition<std::string>(core::tts::kReadyStr)
      );
}

EVBInterface::~EVBInterface() {
}


const std::string& EVBInterface::ttsInternalStatusDecoder(uint32_t aTTSInternalState) {
  /*
   * From: http://bucms.bu.edu/twiki/bin/view/BUCMSPublic/AMC13UserManual
   * 
   * Bit 4 Disconnected
   * Bit 3 Error
   * Bit 2 Sync Lost
   * Bit 1 Busy
   * Bit 0 Overflow Warning
   * 
   * TCDS TTS States: https://twiki.cern.ch/twiki/bin/view/CMS/TcdsTtsStates
*/
  
  const uint32_t kDisconnected = (1<<4);
  const uint32_t kError = (1<<3);
  const uint32_t kSyncLost = (1<<2);
  const uint32_t kBusy = (1<<1);
  const uint32_t kOverflowWarning = 1;
  const uint32_t kReady = 0;
  switch (aTTSInternalState) {
    case kReady:
      return core::tts::kReadyStr;
    case kBusy:
      return core::tts::kBusyStr;
    case kOverflowWarning:
      return core::tts::kWarningStr;
    case kSyncLost:
      return core::tts::kOutOfSyncStr;
    case kError:
      return core::tts::kErrorStr;
    case kDisconnected:
      return core::tts::kDisconnectedStr;
    default:
      return core::tts::kUnknownStr;
  }
}

void 
EVBInterface::retrieveMetricValues() {
  using ::amc13::AMC13;

  const std::string prefixStat = "STATUS.";
  const std::string prefixEvb = "STATUS.EVB.";
  const std::string prefixGeneral = "STATUS.GENERAL.";
  setMetricValue<>(mOverflowWarning, (bool)mDriver.read(AMC13::T1,prefixEvb+"OVERFLOW_WARNING"));
  setMetricValue<>(mSyncLost, (bool)mDriver.read(AMC13::T1,prefixEvb+"SYNC_LOST"));
  setMetricValue<>(mAMCsTTSState, ttsInternalStatusDecoder(mDriver.read(AMC13::T1,prefixStat+"AMC_TTS_STATE")));
  setMetricValue<>(mTTSState, core::tts::codeToString(mDriver.read(AMC13::T1,prefixStat+"T1_TTS_STATE")));
  setMetricValue<>(mL1ACount, read64bCounter(mDriver, AMC13::T1,prefixGeneral+"L1A_COUNT"));
  setMetricValue<>(mRunTime, read64bCounter(mDriver, AMC13::T1,prefixGeneral+"RUN_TIME"));
  setMetricValue<>(mReadyTime, read64bCounter(mDriver, AMC13::T1,prefixGeneral+"READY_TIME"));
  setMetricValue<>(mBusyTime, read64bCounter(mDriver, AMC13::T1,prefixGeneral+"BUSY_TIME"));
  setMetricValue<>(mSyncLostTime, read64bCounter(mDriver, AMC13::T1,prefixGeneral+"SYNC_LOST_TIME"));
  setMetricValue<>(mOverflowWarningTime, read64bCounter(mDriver, AMC13::T1,prefixGeneral+"OF_WARN_TIME"));


}


} // namespace amc13
} // namespace swatch


