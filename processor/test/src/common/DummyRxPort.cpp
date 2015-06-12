/*
 * DummyRxPort.cpp
 */

#include "swatch/processor/test/DummyRxPort.hpp"

namespace swatch {
namespace processor {
namespace test {

DummyRxPort::DummyRxPort(const std::string& aId) :
        InputPort(aId) {
}


DummyRxPort::~DummyRxPort() {
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

void DummyRxPort::implementUpdateMetrics() {
  setMetricValue<>(metricIsEnabled_, true);
  setMetricValue<>(metricIsLocked_, true);
  setMetricValue<>(metricIsAligned_, true);
  setMetricValue<>(metricCRCErrors_, uint32_t(0));
}

} // namespace test
} // namespace processor
} // namespace swatch
