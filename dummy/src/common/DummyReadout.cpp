
#include "swatch/dummy/DummyReadout.hpp"


namespace swatch {
namespace dummy {


DummyReadoutInterface::DummyReadoutInterface(DummyProcDriver& aDriver) :
ReadoutInterface(),
mDriver(aDriver) {
}


DummyReadoutInterface::~DummyReadoutInterface() {
}


void DummyReadoutInterface::retrieveMetricValues() {
  setMetricValue<>(mMetricTTS, uint32_t(0x4));
}


} // namespace dummy
} // namespace swatch
