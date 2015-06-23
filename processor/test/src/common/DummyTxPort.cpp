/*
 * DummyRxPort.cpp
 */

#include "swatch/processor/test/DummyTxPort.hpp"

namespace swatch {
namespace processor {
namespace test {

DummyTxPort::DummyTxPort(const std::string& aId) :
OutputPort(aId){
}

DummyTxPort::~DummyTxPort() {
}

/*
bool DummyTxPort::isEnabled() const {
  return true;
}

bool DummyTxPort::isOperating() const {
  return true;
}
 */

void DummyTxPort::implementUpdateMetrics() {
  setMetricValue<>(metricIsOperating_, true);
}
} // namespace test
} // namespace processor
} // namespace swatch
