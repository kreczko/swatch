
#include "swatch/mp7/MP7ReadoutInterface.hpp"

// MP7 Headers
#include "mp7/MP7Controller.hpp"
#include "mp7/ReadoutNode.hpp"

namespace swatch {
namespace mp7 {


MP7ReadoutInterface::MP7ReadoutInterface(::mp7::MP7Controller& controller) :
mDriver(controller) {
}


MP7ReadoutInterface::~MP7ReadoutInterface() {
}


void MP7ReadoutInterface::retrieveMetricValues() {
  const ::mp7::ReadoutNode& readOut = mDriver.getReadout();

  setMetricValue<>(mMetricTTS, (uint32_t)readOut.readTTSState());
  setMetricValue<>(mMetricAMCCoreReady, (bool)readOut.isAMC13LinkReady());


}

} // namespace mp7
} // namespace swatch
