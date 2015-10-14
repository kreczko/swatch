/* 
 * File:   SLink.cpp
 * Author: ale
 * 
 * Created on October 1, 2015, 3:13 PM
 */


 //     Slink_Express|       SFP0|
 //                --|-----------|
 // BACKPRESSURE_TIME| 0x 170BC7E|
 //            BLOCKS| 0x   32F80|
 //       BLOCKS_FREE|          1|
 //       BLOCKS_SENT| 0x   33EBE|
 //  BLOCK_READY_MASK|          0|
 //   BLOCK_USED_MASK|          0|
 //       CORE_STATUS| 0x80000000|
 //            EVENTS| 0x   32F80|
 //       EVENTS_SENT| 0x   32F80|
 //    EVENT_LGTH_ERR| 0x    A799|
 //    EVENT_LGTH_SUM| 0x 3A25B78|
 //          EXPERT_1|          0|
 //          EXPERT_2| 0x   2F4BD|
 //          FSM_IDLE|          1|
 //       INITIALIZED|          1|
 //       LINKNOTFULL|          0|
 //           LINK_UP|          1|
 //   NO_BACKPRESSURE|          1|
 //      PACKETS_RCVD| 0x   3373B|
 //      PACKETS_SENT| 0x 7724E55|
 //          REVISION| 0x5E000008|
 //      SFP_LSC_DOWN|          0|
 //          STATUS_D| 0x70000001|
 //         SYNC_LOSS| 0x2EF05FC7|
 //         TEST_MODE|          0|
 //             WORDS| 0x 3A25B78|
 //        WORDS_SENT| 0x 3A25B78|

#include "swatch/amc13/SLinkExpress.hpp"

#include "swatch/core/MetricConditions.hpp"
#include "swatch/core/Utilities.hpp"

// AMC13 Headers
#include "amc13/AMC13.hh"

namespace swatch {
namespace amc13 {
  
// --------------------------------------------------------
SLinkExpress::SLinkExpress( uint32_t aSfpId, ::amc13::AMC13& aDriver ) : 
  dtm::SLinkExpress(aSfpId),
  mDriver(aDriver),
  mRevision(registerMetric<uint32_t>("revision") ),
  mCoreInitialised(registerMetric<bool>("initialized",core::EqualCondition<bool>(false)) ),
  mLinkUp(registerMetric<bool>("linkUp",core::EqualCondition<bool>(false)) ),
  mBackPressure(registerMetric<bool>("backPressure",core::EqualCondition<bool>(true)) ),
  mBackPressureTime(registerMetric<uint32_t>("backPressureTime") ) ,
  mEvents(registerMetric<uint32_t>("events") ),
  mEventsSent(registerMetric<uint32_t>("eventsSent") ),
  mWords(registerMetric<uint32_t>("words") ),
  mWordsSent(registerMetric<uint32_t>("wordsSent") ),
  mPacketsSent(registerMetric<uint32_t>("packetsSent") ),
  mPacketsReceived(registerMetric<uint32_t>("packetsReceived") ) {
}


// --------------------------------------------------------
SLinkExpress::~SLinkExpress() {
}


// --------------------------------------------------------
void SLinkExpress::retrieveMetricValues() {
  using ::amc13::AMC13;

  const std::string prefixA = swatch::core::strPrintf("STATUS.SFP.SFP%d.LSC.", sfpID);
  const std::string prefixB = swatch::core::strPrintf("STATUS.LSC.SFP%d.", sfpID);
  setMetricValue<>(mRevision, mDriver.read(AMC13::T1,prefixA+"VERSION"));
  setMetricValue<>(mCoreInitialised, (bool)mDriver.read(AMC13::T1,prefixA+"INITIALIZED"));
  setMetricValue<>(mLinkUp, (bool)mDriver.read(AMC13::T1,prefixA+"LINK_UP"));
  setMetricValue<>(mBackPressure, !(bool)mDriver.read(AMC13::T1,prefixA+"NO_BACKPRESSURE"));
  setMetricValue<>(mBackPressureTime, mDriver.read(AMC13::T1,prefixA+"BACKP_COUNTER"));
  setMetricValue<>(mEvents, mDriver.read(AMC13::T1,prefixB+"BUILT_EVENT_COUNT"));
  setMetricValue<>(mEventsSent, mDriver.read(AMC13::T1,prefixA+"EVENTS_SENT"));
  setMetricValue<>(mWords, mDriver.read(AMC13::T1,prefixB+"WORD_COUNT"));
  setMetricValue<>(mWordsSent, mDriver.read(AMC13::T1,prefixA+"WORDS_SENT"));
  setMetricValue<>(mPacketsSent, mDriver.read(AMC13::T1,prefixA+"PACKETS_SENT"));
  setMetricValue<>(mPacketsReceived, mDriver.read(AMC13::T1,prefixA+"PACKETS_RCVD"));
  // "PACKETS_SENT";
  // "PACKETS_RCVD";
  // "EVENT_LGTH_ERR";
  // "EVENT_LGTH_SUM";

}


} // namespace amc13
} // namespace swatch 