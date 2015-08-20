
#ifndef __SWATCH_PROCESSOR_TEST_DUMMYREADOUT_HPP__
#define __SWATCH_PROCESSOR_TEST_DUMMYREADOUT_HPP__

#include <string>

#include "swatch/processor/ReadoutInterface.hpp"


namespace swatch {
namespace processor {
namespace test {

class DummyDriver;

/**
 * @class DummyReadoutInterface
 * @brief Dummy readout interface implementation
 */
class DummyReadoutInterface : public ReadoutInterface {
public:
  DummyReadoutInterface(DummyDriver& aDriver);

  virtual ~DummyReadoutInterface();

  virtual void retrieveMetricValues();

private:
  DummyDriver& driver_;
};

} // namespace test
} // namespace processor
} // namespace swatch

#endif /* SWATCH_PROCESSOR_TEST_DUMMYREADOUT_HPP */
