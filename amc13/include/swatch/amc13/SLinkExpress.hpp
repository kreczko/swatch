/* 
 * File:   SLinkExpress.hpp
 * Author: ale
 *
 * Created on October 1, 2015, 3:12 PM
 */

#ifndef __SWATCH_AMC13_SLINK_HPP__
#define	__SWATCH_AMC13_SLINK_HPP__

#include "swatch/core/MonitorableObject.hpp"
#include "swatch/dtm/SLinkExpress.hpp"

namespace amc13 {
  class AMC13;
} // namespace amc13

namespace swatch {
namespace amc13 {

class SLinkExpress : public dtm::SLinkExpress {
public:
  SLinkExpress( uint32_t aSfpID, ::amc13::AMC13& aDriver );
  virtual ~SLinkExpress();

protected:
    virtual void retrieveMetricValues();

private:
  ::amc13::AMC13& mDriver;

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

} // namespace amc13
} // namespace swatch 

#endif	/* __SWATCH_AMC13_SLINK_HPP__ */

