
#include "swatch/processor/test/DummyTTC.hpp"
#include "swatch/processor/test/DummyDriver.hpp"

namespace swatch {
namespace processor {
namespace test {

DummyTTC::DummyTTC(DummyDriver& aDriver) :
  TTCInterface(),
  driver_(aDriver)
{
}


DummyTTC::~DummyTTC()
{
}


void DummyTTC::retrieveMetricValues()
{
  DummyDriver::TTCStatus s = driver_.getTTCStatus();
  
  setMetricValue<>(metricBunchCounter_, s.bunchCounter);
  setMetricValue<>(metricEventCounter_, s.eventCounter);
  setMetricValue<>(metricOrbitCounter_, s.orbitCounter);

  setMetricValue<>(metricIsClock40Locked_, s.clk40Locked);
  setMetricValue<>(metricHasClock40Stopped_, s.clk40Stopped);
  setMetricValue<>(metricIsBC0Locked_, s.bc0Locked);

  setMetricValue<>(metricSingleBitErrors_, s.errSingleBit);
  setMetricValue<>(metricDoubleBitErrors_, s.errDoubleBit);
}

} // namespace test
} // namespace processor
} // namespace swatch
