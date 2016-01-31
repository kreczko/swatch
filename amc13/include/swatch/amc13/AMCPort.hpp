/* 
 * File:   AMCLink.hpp
 * Author: ale
 *
 * Created on October 5, 2015, 5:00 PM
 */

#ifndef __SWATCH_AMC13_AMCPORT_HPP__
#define	__SWATCH_AMC13_AMCPORT_HPP__

#include "swatch/dtm/AMCPort.hpp"
// #include "amc13/AMC13.hh"

namespace amc13 {
  class AMC13;
} // namespace amc13


namespace swatch {
namespace amc13 {

class AMCPort : public dtm::AMCPort {
public:
  AMCPort( uint32_t aSlot, ::amc13::AMC13& aDriver );
  virtual ~AMCPort();


protected:
  virtual void retrieveMetricValues();

  ::amc13::AMC13& mDriver;
  
    //! AMC_Links: AMC_LINK_VER
  core::Metric<uint32_t>& mAMCLinkRevision;
  
  //! AMC_Links: LINK_VERS_WRONG
  core::Metric<bool>& mLinkRevisionWrong;

  //! AMC_Links: AMC_LINK_READY
  core::Metric<bool>& mLinkReady;

  //! AMC_Links: LINK_OK
  core::Metric<bool>& mLinkOK;

  //! AMC_Links: AMC13_LINK_VER
  core::Metric<uint32_t>& mAMC13LinkRevision;

  //! AMC_Links: AMC_TTS
  core::Metric<uint32_t>& mTTS;

  //! AMC_Links: AMC_EVENTS
  core::Metric<uint64_t>& mAMCEvents;

  //! AMC_Links: AMC_HEADERS
  core::Metric<uint64_t>& mAMCHeaders;

  //! AMC_Links: AMC_TRAILERS
  core::Metric<uint64_t>& mAMCTrailers;

  //! AMC_Links: AMC_WORDS
  core::Metric<uint64_t>& mAMCWords;

  //! AMC_Links: AMC13_EVENTS
  core::Metric<uint64_t>& mAMC13Events;

  //! AMC_Links: AMC13_WORDS
  core::Metric<uint64_t>& mAMC13Words;

  //! AMC_Links: AMC_BCN_MISMATCH
  core::Metric<uint64_t>& mAMCBcnMismatch;

  //! AMC_Links: AMC_ORN_MISMATCH
  core::Metric<uint64_t>& mAMCOrnMismatch;

  //! AMC_Links: AMC13_BCN_MISMATCH
  core::Metric<uint64_t>& mAMC13BcnMismatch;

  //! AMC_Links: AMC13_ORN_MISMATCH
  core::Metric<uint64_t>& mAMC13OrnMismatch;

  //! AMC_Links: AMC_EVN_ERRORS
  core::Metric<uint64_t>& mEventErrors;

  //! AMC_Links: CRC_ERR
  core::Metric<uint32_t>& mCrcErrors;

};

} // namespace amc13
} // namespace swatch 

#endif	/* __SWATCH_AMC13_AMCPORT_HPP__ */

