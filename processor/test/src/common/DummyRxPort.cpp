
#include "swatch/processor/test/DummyRxPort.hpp"
#include "swatch/processor/test/DummyDriver.hpp"

namespace swatch {
namespace processor {
namespace test {

DummyRxPort::DummyRxPort(const std::string& aId, uint32_t aNumber, DummyDriver& aDriver) :
  InputPort(aId),
  channelID_(aNumber),
  driver_(aDriver)
{
}


DummyRxPort::~DummyRxPort()
{
}

/*
bool DummyRxPort::isEnabled() const {
  return true;
}


bool DummyRxPort::isLocked() const {
  return true;
}


bool DummyRxPort::isAligned() const {
  return true;
}


uint32_t DummyRxPort::getCRCErrors() const {
  return 0;
}
 */

void DummyRxPort::retrieveMetricValues()
{
  DummyDriver::RxPortStatus s = driver_.getRxPortStatus(channelID_);
  
  setMetricValue<>(metricIsLocked_, s.isLocked);
  setMetricValue<>(metricIsAligned_, s.isAligned);
  setMetricValue<>(metricCRCErrors_, s.crcErrCount);
}

} // namespace test
} // namespace processor
} // namespace swatch
