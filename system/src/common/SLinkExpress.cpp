/* 
 * File:   SLink.cpp
 * Author: ale
 * 
 * Created on October 1, 2015, 3:13 PM
 */


#include "swatch/dtm/SLinkExpress.hpp"

#include "swatch/core/MetricConditions.hpp"
#include "swatch/core/Utilities.hpp"

namespace swatch {
namespace dtm {
  
// --------------------------------------------------------
SLinkExpress::SLinkExpress( uint32_t aSfpId ) : 
  swatch::core::MonitorableObject(swatch::core::strPrintf("slinkXp%d",aSfpId)),
  sfpID(aSfpId),
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


} // namespace dtm
} // namespace swatch 