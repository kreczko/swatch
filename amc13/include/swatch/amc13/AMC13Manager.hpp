/**
 * @file    AMC13Manager.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    07/11/14
 */

#ifndef SWATCH_AMC13_AMC13MANAGER_HPP
#define SWATCH_AMC13_AMC13MANAGER_HPP


// Swatch Headers
#include "swatch/system/DaqTTCManager.hpp"

// Forward declaration
namespace amc13 {
class AMC13;
}

namespace swatch {
namespace amc13 {

class TTCInterface;

class AMC13Manager : public swatch::system::DaqTTCManager {
public:
  AMC13Manager(const swatch::core::AbstractStub& aStub);
  ~AMC13Manager();

  ::amc13::AMC13& driver() {
    return *mDriver;
  }

protected:

  virtual void retrieveMetricValues();

  //! Register the supplied (heap-allocated) TTC interface in this processor; the processor base class takes ownership of the TTC interface instance.
  TTCInterface& registerInterface( TTCInterface* aTTCInterface );

private:
  ::amc13::AMC13* mDriver;

  TTCInterface* mTTC;

  swatch::core::Metric<uint32_t>& mFwVersionT1;
  swatch::core::Metric<uint32_t>& mFwVersionT2_;
};

DEFINE_SWATCH_EXCEPTION(DaqTTCManagerInterfaceAlreadyDefined);

} // namespace amc13
} // namespace swatch

#endif /* SWATCH_AMC13_AMC13MANAGER_HPP */
