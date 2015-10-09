/* 
 * File:   SLinkExpress.hpp
 * Author: ale
 *
 * Created on October 1, 2015, 3:12 PM
 */

#ifndef __SWATCH_DTM_SLINK_HPP__
#define	__SWATCH_DTM_SLINK_HPP__

#include "swatch/core/MonitorableObject.hpp"

namespace swatch {
namespace dtm {

class SLinkExpress : public core::MonitorableObject {
public:
  SLinkExpress( uint32_t aSfpID );
  virtual ~SLinkExpress();

protected:

  uint32_t sfpID;
  
  core::Metric<uint32_t>& mRevision;
  core::Metric<bool>& mCoreInitialised;
  core::Metric<bool>& mLinkUp;
  core::Metric<bool>& mBackPressure;
  core::Metric<uint32_t>& mBackPressureTime;
  core::Metric<uint32_t>& mEvents;
  core::Metric<uint32_t>& mEventsSent;
  core::Metric<uint32_t>& mWords;
  core::Metric<uint32_t>& mWordsSent;
  core::Metric<uint32_t>& mPacketsSent;
  core::Metric<uint32_t>& mPacketsReceived;

};

} // namespace dtm
} // namespace swatch 

#endif	/* __SWATCH_DTM_SLINK_HPP__ */

