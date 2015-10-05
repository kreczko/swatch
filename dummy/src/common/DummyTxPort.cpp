
#include "swatch/dummy/DummyTxPort.hpp"
#include "swatch/dummy/DummyProcDriver.hpp"


namespace swatch {
namespace dummy {


DummyTxPort::DummyTxPort(const std::string& aId, uint32_t aNumber, DummyProcDriver& aDriver) :
  OutputPort(aId),
  channelID_(aNumber),
  driver_(aDriver)
{
}

DummyTxPort::~DummyTxPort()
{
}


void DummyTxPort::retrieveMetricValues()
{
  setMetricValue<>(metricIsOperating_, driver_.isTxPortOperating(channelID_));
}


} // namespace dummy
} // namespace swatch
