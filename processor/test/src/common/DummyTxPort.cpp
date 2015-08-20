
#include "swatch/processor/test/DummyTxPort.hpp"
#include "swatch/processor/test/DummyDriver.hpp"


namespace swatch {
namespace processor {
namespace test {

DummyTxPort::DummyTxPort(const std::string& aId, uint32_t aNumber, DummyDriver& aDriver) :
  OutputPort(aId),
  channelID_(aNumber),
  driver_(aDriver)
{
}

DummyTxPort::~DummyTxPort()
{
}

/*
bool DummyTxPort::isEnabled() const {
  return true;
}

bool DummyTxPort::isOperating() const {
  return true;
}
 */

void DummyTxPort::retrieveMetricValues()
{
  setMetricValue<>(metricIsOperating_, driver_.isTxPortOperating(channelID_));
}

} // namespace test
} // namespace processor
} // namespace swatch
