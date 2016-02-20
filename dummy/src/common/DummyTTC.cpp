
#include "swatch/dummy/DummyTTC.hpp"
#include "swatch/dummy/DummyProcDriver.hpp"
#include "swatch/core/MetricConditions.hpp"


namespace swatch {
namespace dummy {


DummyTTC::DummyTTC(DummyProcDriver& aDriver) :
  TTCInterface(),
  mDriver(aDriver),
  mWarningSign(registerMetric<bool>("warningSign"))
{
  setWarningCondition<>(mWarningSign, core::EqualCondition<bool>(true));
}


DummyTTC::~DummyTTC()
{
}


void DummyTTC::retrieveMetricValues()
{
  DummyProcDriver::TTCStatus s = mDriver.getTTCStatus();
  
  setMetricValue<>(mMetricL1ACounter, s.eventCounter);
  setMetricValue<>(mMetricBunchCounter, s.bunchCounter);
  setMetricValue<>(mMetricOrbitCounter, s.orbitCounter);

  setMetricValue<>(mMetricIsClock40Locked, s.clk40Locked);
  setMetricValue<>(mMetricHasClock40Stopped, s.clk40Stopped);
  setMetricValue<>(mMetricIsBC0Locked, s.bc0Locked);

  setMetricValue<>(mMetricSingleBitErrors, s.errSingleBit);
  setMetricValue<>(mMetricDoubleBitErrors, s.errDoubleBit);

  setMetricValue<>(mWarningSign, s.warningSign);
}


} // namespace dummy
} // namespace swatch
