/* 
 * File:   DAQInterface.hpp
 * Author: ale
 *
 * Created on September 21, 2015, 12:21 PM
 */

#ifndef __SWATCH_AMC13_DAQINTERFACE_HPP__
#define	__SWATCH_AMC13_DAQINTERFACE_HPP__


#include "swatch/dtm/EVBInterface.hpp"

namespace amc13 {
class AMC13;
} // namespace amc13

namespace swatch {
namespace amc13 {

class EVBInterface : public dtm::EVBInterface {
public:
  EVBInterface(::amc13::AMC13& aDriver);
  virtual ~EVBInterface();


  virtual void retrieveMetricValues();

private:
  static uint32_t ttsInternalStatusDecoder( uint32_t aTTSInternalState );

  ::amc13::AMC13& mDriver;

  // T1: event data ready in event buffer of event builders [STATUS.EVB.DATA_READY_MASK]
  // T1: event size in event buffer of event builders [STATUS.EVB.EVENT_SIZE_MASK]

  // T1: L1A overflow warning [STATUS.EVB.OVERFLOW_WARNING]
  core::Metric<bool>& mOverflowWarning;

  // T1: TTC sync lost (L1A buffer overflow) [STATUS.EVB.SYNC_LOST]
  core::Metric<bool>& mSyncLost;

  // T1: STATUS.AMC_TTS_STATE
  core::Metric<uint32_t>& mAMCsTTSState;

  // T1: STATUS.T1_TTS_STATE
  core::Metric<uint32_t>& mTTSState;

  // T1: STATUS.GENERAL.L1A_COUNT
  core::Metric<uint64_t>& mL1ACount;

  // T1: STATUS.GENERAL.RUN_TIME
  core::Metric<uint64_t>& mRunTime;

  // T1: STATUS.GENERAL.READY_TIME
  core::Metric<uint64_t>& mReadyTime;

  // T1: STATUS.GENERAL.BUSY_TIME
  core::Metric<uint64_t>& mBusyTime;

  // T1: STATUS.GENERAL.SYNC_LOST_TIME
  core::Metric<uint64_t>& mSyncLostTime;

  // T1: STATUS.GENERAL.OF_WARN_TIME
  core::Metric<uint64_t>& mOverflowWarningTime;

  // T1: 

};

} // namespace amc13
} // namespace swatch


#endif	/* __SWATCH_AMC13_DAQINTERFACE_HPP__ */

