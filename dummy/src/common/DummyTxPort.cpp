
#include "swatch/dummy/DummyTxPort.hpp"
#include "swatch/dummy/DummyProcDriver.hpp"


namespace swatch {
namespace dummy {


DummyTxPort::DummyTxPort(const std::string& aId, uint32_t aNumber, DummyProcDriver& aDriver) :
  OutputPort(aId),
  mChannelId(aNumber),
  mDriver(aDriver)
{
}

DummyTxPort::~DummyTxPort()
{
}


void DummyTxPort::retrieveMetricValues()
{
  setMetricValue<>(metricIsOperating_, mDriver.isTxPortOperating(mChannelId));
}


} // namespace dummy
} // namespace swatch
