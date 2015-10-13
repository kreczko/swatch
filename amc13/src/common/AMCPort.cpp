/* 
 * File:   AMCLink.cpp
 * Author: ale
 * 
 * Created on October 5, 2015, 5:00 PM
 */

#include "swatch/amc13/AMCPort.hpp"

#include "swatch/core/MetricConditions.hpp"
#include "swatch/core/Utilities.hpp"

 // AMC13 Headers
#include "amc13/AMC13.hh"

 //          AMC_Links|       AMC08|
 //                 --|------------|
 // AMC13_BCN_MISMATCH| 0x 112E8916|
 //       AMC13_EVENTS| 0x 112E8916|
 //     AMC13_LINK_VER| 0x       11|
 // AMC13_ORN_MISMATCH| 0x 112E8916|
 //        AMC13_WORDS| 0xAABE724AA|
 //   AMC_BCN_MISMATCH| 0x 112E8911|
 //         AMC_EVENTS| 0x 112E8911|
 //        AMC_HEADERS| 0x 112E8907|
 //     AMC_LINK_READY|           1|
 //       AMC_LINK_VER| 0x       10|
 //   AMC_ORN_MISMATCH| 0x 112E8911|
 //       AMC_TRAILERS| 0x 112E8907|
 //            AMC_TTS|   RDY (0x8)|
 //          AMC_WORDS| 0xAABE71B59|
 //           BC0_LOCK|           0|
 //         BC0_LOCKED|           0|
 //         EVB_EVENTS| 0x 112E8916|
 //            LINK_OK|           1|
 //    LINK_VERS_WRONG|           1|
 //       MISMATCH_BCN|           0|
 //       MISMATCH_ORN|           0|
 //       MISMATCH_TTS|           8|
 //           TTC_LOCK|           0|
 //            TTS_RAW|   DIS (0x0)|
 //         TTS_UPDATE| 0x12B9B24C9|
// AMC_EVN_ERRORS
// CRC_ERR
// BP_ERR
namespace swatch {
namespace amc13 {

AMCPort::AMCPort(uint32_t aSlot, ::amc13::AMC13& aDriver) :
  dtm::AMCPort(aSlot),
  mDriver(aDriver) {
}

AMCPort::~AMCPort() {
}


uint64_t 
AMCPort::read64bCounter(::amc13::AMC13::Board aBoard, const std::string& aName) {
  return  
      ((uint64_t)mDriver.read(aBoard,aName+"_HI") << 32) +
      mDriver.read(aBoard,aName+"_LO");
}

void AMCPort::retrieveMetricValues() {

  using ::amc13::AMC13;

  const std::string prefixAmc = swatch::core::strPrintf("STATUS.AMC%02d.", mSlotId);
  const std::string prefixLink = swatch::core::strPrintf("STATUS.AMC%02d.LINK.", mSlotId);
  const std::string prefixCtrs = swatch::core::strPrintf("STATUS.AMC%02d.COUNTERS.", mSlotId);

  setMetricValue<>(mAMCLinkRevision, mDriver.read(AMC13::T1,prefixLink+"AMC_LINK_VER"));

  setMetricValue<>(mLinkRevisionWrong, (bool)mDriver.read(AMC13::T1,prefixAmc+"LINK_VERS_WRONG_MASK"));
  
  setMetricValue<>(mLinkReady, (bool)mDriver.read(AMC13::T1,prefixAmc+"AMC_LINK_READY_MASK"));
  
  setMetricValue<>(mLinkOK, (bool)mDriver.read(AMC13::T1,prefixAmc+"LOSS_OF_SYNC_MASK"));
  
  setMetricValue<>(mAMC13LinkRevision, mDriver.read(AMC13::T1,prefixLink+"AMC13_LINK_VER"));
  
  setMetricValue<>(mTTS, mDriver.read(AMC13::T1,prefixLink+"AMC_TTS"));

  // Counters
  setMetricValue<>(mAMCEvents, read64bCounter(AMC13::T1,prefixCtrs+"RECEIVED_EVENT_COUNTER"));
  
  setMetricValue<>(mAMCHeaders, read64bCounter(AMC13::T1,prefixCtrs+"HEADERS_AT_LINK_INPUT"));
  
  setMetricValue<>(mAMCTrailers, read64bCounter(AMC13::T1,prefixCtrs+"TRAILERS_AT_LINK_INPUT"));
  
  setMetricValue<>(mAMCWords, read64bCounter(AMC13::T1,prefixCtrs+"WORDS_AT LINK_INPUT"));

  setMetricValue<>(mAMC13Events, read64bCounter(AMC13::T1,prefixCtrs+"AMC13_AMC_EVENTS"));

  setMetricValue<>(mAMC13Words, read64bCounter(AMC13::T1,prefixCtrs+"WORD_COUNTER"));

  setMetricValue<>(mAMCBcnMismatch, read64bCounter(AMC13::T1,prefixCtrs+"BCN_MISMATCH_COUNTER"));

  setMetricValue<>(mAMCOrnMismatch, read64bCounter(AMC13::T1,prefixCtrs+"ORN_MISMATCH_COUNTER"));

  setMetricValue<>(mAMC13BcnMismatch, read64bCounter(AMC13::T1,prefixCtrs+"AMC13_BCN_MISMATCH"));

  setMetricValue<>(mAMC13OrnMismatch, read64bCounter(AMC13::T1,prefixCtrs+"AMC13_ORN_MISMATCH"));

  setMetricValue<>(mEventErrors, read64bCounter(AMC13::T1,prefixCtrs+"EVN_ERRORS_AT_LINK_INPUT"));

  setMetricValue<>(mCrcErrors, mDriver.read(AMC13::T1,prefixAmc+"BP_CRC_ERR"));


}




} // namespace amc13
} // namespace swatch 