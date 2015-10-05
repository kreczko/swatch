
#include "swatch/dummy/DummyReadout.hpp"


namespace swatch {
namespace dummy {


DummyReadoutInterface::DummyReadoutInterface(DummyProcDriver& aDriver) :
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


} // namespace dummy
} // namespace swatch
