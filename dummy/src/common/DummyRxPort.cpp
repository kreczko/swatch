
#include "swatch/dummy/DummyRxPort.hpp"


#include "swatch/dummy/DummyProcDriver.hpp"


namespace swatch {
namespace dummy {


DummyRxPort::DummyRxPort(const std::string& aId, uint32_t aNumber, DummyProcDriver& aDriver) :
  InputPort(aId),
  channelID_(aNumber),
  driver_(aDriver)
{
}


DummyRxPort::~DummyRxPort()
{
}


void DummyRxPort::retrieveMetricValues()
{
  DummyProcDriver::RxPortStatus s = driver_.getRxPortStatus(channelID_);
  
  setMetricValue<>(metricIsLocked_, s.isLocked);
  setMetricValue<>(metricIsAligned_, s.isAligned);
  setMetricValue<>(metricCRCErrors_, s.crcErrCount);
}


} // namespace dummy
} // namespace swatch
