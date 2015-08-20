
#include "swatch/processor/test/DummyReadout.hpp"


namespace swatch {
namespace processor {
namespace test {

DummyReadoutInterface::DummyReadoutInterface(DummyDriver& aDriver) :
  ReadoutInterface(),
  driver_(aDriver)
{
}


DummyReadoutInterface::~DummyReadoutInterface()
{
}


void DummyReadoutInterface::retrieveMetricValues()
{
  //TODO: extract status from driver
//  setMetricValue<>(metricIsLocked_, true);
//  setMetricValue<>(metricIsAligned_, true);
//  setMetricValue<>(metricCRCErrors_, uint32_t(0));
}

} // namespace test
} // namespace processor
} // namespace swatch
