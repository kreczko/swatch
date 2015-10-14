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
  
protected:

  virtual void retrieveMetricValues();


private:
  ::amc13::AMC13* mDriver;

  swatch::core::Metric<uint32_t>& mFwVersionT1;
  swatch::core::Metric<uint32_t>& mFwVersionT2;
};


} // namespace amc13
} // namespace swatch

#endif /* SWATCH_AMC13_AMC13MANAGER_HPP */
