/* 
 * File:   AMCLink.cpp
 * Author: ale
 * 
 * Created on October 5, 2015, 5:00 PM
 */

#include "swatch/dtm/AMCPort.hpp"

#include "swatch/core/MetricConditions.hpp"
#include "swatch/core/Utilities.hpp"

namespace swatch {
namespace dtm {

AMCPort::AMCPort(uint32_t aSlot) :
  swatch::core::MonitorableObject(swatch::core::strPrintf("amc%02d",aSlot)),
  mSlotId(aSlot),
  mAMCLinkRevision(registerMetric<uint32_t>("amcRevision") ),
  mLinkRevisionWrong(registerMetric<bool>("revisionWrong",core::EqualCondition<bool>(true)) ),
  mLinkReady(registerMetric<bool>("ready",core::EqualCondition<bool>(false)) ),
  mLinkOK(registerMetric<bool>("ok",core::EqualCondition<bool>(false)) ),
  mAMC13LinkRevision(registerMetric<uint32_t>("amc13Revision") ),
  mTTS(registerMetric<uint32_t>("tts",core::NotEqualCondition<uint32_t>(0x8)) ) ,
  
  mAMCEvents(registerMetric<uint64_t>("amcEvents") ) ,
  mAMCHeaders(registerMetric<uint64_t>("amcHeaders") ) ,
  mAMCTrailers(registerMetric<uint64_t>("amcTrailers") ) ,
  mAMCWords(registerMetric<uint64_t>("amcWords") ) ,

  mAMC13Events(registerMetric<uint64_t>("amc13Events") ) ,
  mAMC13Words(registerMetric<uint64_t>("amc13Words") ) ,

  mAMCBcnMismatch(registerMetric<uint64_t>("amcBcnMismatch",core::NotEqualCondition<uint64_t>(0x0)) ) ,
  mAMCOrnMismatch(registerMetric<uint64_t>("amcOrnMismatch",core::NotEqualCondition<uint64_t>(0x0)) ) ,
  mAMC13BcnMismatch(registerMetric<uint64_t>("amc13BcnMismatch",core::NotEqualCondition<uint64_t>(0x0)) ) ,
  mAMC13OrnMismatch(registerMetric<uint64_t>("amc13OrnMismatch",core::NotEqualCondition<uint64_t>(0x0)) ) ,

  mEventErrors(registerMetric<uint64_t>("eventErrors",core::NotEqualCondition<uint64_t>(0x0)) ) ,
  mCrcErrors(registerMetric<uint32_t>("crcErrors", core::NotEqualCondition<uint32_t>(0x0))) {
}

AMCPort::~AMCPort() {
}


} // namespace amc13
} // namespace swatch 
