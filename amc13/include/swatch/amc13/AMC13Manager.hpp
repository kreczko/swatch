/**
 * @file    AMC13Manager.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    07/11/14
 */

#ifndef SWATCH_AMC13_AMC13MANAGER_HPP
#define SWATCH_AMC13_AMC13MANAGER_HPP


// Swatch Headers
#include "swatch/dtm/DaqTTCManager.hpp"

// Forward declaration
namespace amc13 {
class AMC13;
}


namespace swatch {

namespace dtm {
class AMCPortCollection;
}

namespace amc13 {

class TTCInterface;
class SLinkExpress;

class AMC13Manager : public swatch::dtm::DaqTTCManager {
public:
  AMC13Manager(const swatch::core::AbstractStub& aStub);
  ~AMC13Manager();

  ::amc13::AMC13& driver() {
    return *mDriver;
  }
  
      //! Returns this amc13's TTC interface
    TTCInterface& getTTC();
    
    //! Returns this amc13's link interface
    dtm::AMCPortCollection& getAMCPorts();

protected:

  virtual void retrieveMetricValues();

  //! Register the supplied (heap-allocated) TTC interface in this processor; the processor base class takes ownership of the TTC interface instance.
  TTCInterface& registerInterface( TTCInterface* aTTCInterface );

  //! Register the supplied (heap-allocated) SLink interface in this processor; the processor base class takes ownership of the TTC interface instance.
  SLinkExpress& registerInterface( SLinkExpress* aSLink );

  //! Register the supplied (heap-allocated) link interface in this processor; the processor base class takes ownership of the link interface instance.
  dtm::AMCPortCollection& registerInterface( dtm::AMCPortCollection* aPortCollection );

private:
  ::amc13::AMC13* mDriver;

  TTCInterface* mTTC;
  SLinkExpress* mSLink;
  dtm::AMCPortCollection* mAMCPorts;

  swatch::core::Metric<uint32_t>& mFwVersionT1;
  swatch::core::Metric<uint32_t>& mFwVersionT2_;
};

DEFINE_SWATCH_EXCEPTION(DaqTTCManagerInterfaceAlreadyDefined);

} // namespace amc13
} // namespace swatch

#endif /* SWATCH_AMC13_AMC13MANAGER_HPP */
