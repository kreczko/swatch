
#include "swatch/dummy/DummyRxPort.hpp"


#include "swatch/core/MetricConditions.hpp"
#include "swatch/dummy/DummyProcDriver.hpp"


namespace swatch {
namespace dummy {


DummyRxPort::DummyRxPort(const std::string& aId, uint32_t aNumber, DummyProcDriver& aDriver) :
  InputPort(aId),
  mChannelId(aNumber),
  mDriver(aDriver),
  mWarningSign(registerMetric<bool>("warningSign"))
{
  setWarningCondition<>(mWarningSign, core::EqualCondition<bool>(true));
}


DummyRxPort::~DummyRxPort()
{
}


void DummyRxPort::retrieveMetricValues()
{
  DummyProcDriver::RxPortStatus s = mDriver.getRxPortStatus(mChannelId);
  
  setMetricValue<>(mMetricIsLocked, s.isLocked);
  setMetricValue<>(mMetricIsAligned, s.isAligned);
  setMetricValue<>(mMetricCRCErrors, s.crcErrCount);
  setMetricValue<>(mWarningSign, s.warningSign);
}


} // namespace dummy
} // namespace swatch
