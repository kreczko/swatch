
#ifndef __SWATCH_PROCESSOR_TEST_DUMMYALGO_HPP__
#define __SWATCH_PROCESSOR_TEST_DUMMYALGO_HPP__


#include <string>

#include "swatch/processor/AlgoInterface.hpp"


namespace swatch {
namespace processor {
namespace test {

class DummyDriver;

//! Dummy algo interface implementation (used for testing)
class DummyAlgo : public AlgoInterface {
public:
  DummyAlgo(DummyDriver& aDriver);

  virtual ~DummyAlgo();

  virtual void retrieveMetricValues();
  
private:
  DummyDriver& driver_;
};

} // namespace test
} // namespace processor
} // namespace swatch

#endif /* SWATCH_PROCESSOR_TEST_DUMMYALGO_HPP */
