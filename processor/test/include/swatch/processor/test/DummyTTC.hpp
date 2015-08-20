
#ifndef __SWATCH_PROCESSOR_TEST_DUMMYTTC_HPP__
#define __SWATCH_PROCESSOR_TEST_DUMMYTTC_HPP__


#include <string>

#include "swatch/processor/TTCInterface.hpp"


namespace swatch {
namespace processor {
namespace test {

class DummyDriver;

//! Dummy TTC interface implementation (used for testing)
class DummyTTC : public TTCInterface {
public:
  DummyTTC(DummyDriver& aDriver);

  virtual ~DummyTTC();

  virtual void retrieveMetricValues();
  
private:
  DummyDriver& driver_;
};

} // namespace test
} // namespace processor
} // namespace swatch

#endif /* SWATCH_PROCESSOR_TEST_DUMMYTTC_HPP */
