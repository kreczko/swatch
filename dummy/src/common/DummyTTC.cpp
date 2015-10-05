
#include "swatch/dummy/DummyTTC.hpp"
#include "swatch/dummy/DummyProcDriver.hpp"


namespace swatch {
namespace dummy {


DummyTTC::DummyTTC(DummyProcDriver& aDriver) :
  TTCInterface(),
  driver_(aDriver)
{
}


DummyTTC::~DummyTTC()
{
}


void DummyTTC::retrieveMetricValues()
{
  DummyProcDriver::TTCStatus s = driver_.getTTCStatus();
  
  setMetricValue<>(metricBunchCounter_, s.bunchCounter);
  setMetricValue<>(metricEventCounter_, s.eventCounter);
  setMetricValue<>(metricOrbitCounter_, s.orbitCounter);

  setMetricValue<>(metricIsClock40Locked_, s.clk40Locked);
  setMetricValue<>(metricHasClock40Stopped_, s.clk40Stopped);
  setMetricValue<>(metricIsBC0Locked_, s.bc0Locked);

  setMetricValue<>(metricSingleBitErrors_, s.errSingleBit);
  setMetricValue<>(metricDoubleBitErrors_, s.errDoubleBit);
}


} // namespace dummy
} // namespace swatch
