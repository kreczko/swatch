
#ifndef __SWATCH_DUMMY_DUMMYTTC_HPP__
#define __SWATCH_DUMMY_DUMMYTTC_HPP__


#include "swatch/processor/TTCInterface.hpp"


namespace swatch {
namespace dummy {

class DummyProcDriver;

//! Dummy TTC interface implementation (used for testing)
class DummyTTC : public processor::TTCInterface {
public:
  DummyTTC(DummyProcDriver& aDriver);

  virtual ~DummyTTC();

  virtual void retrieveMetricValues();
  
private:
  DummyProcDriver& driver_;
};

} // namespace dummy
} // namespace swatch

#endif /* SWATCH_DUMMY_DUMMYTTC_HPP */
