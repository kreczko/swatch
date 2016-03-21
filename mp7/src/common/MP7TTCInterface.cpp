/* 
 * File:   MP7TCCInterface.hpp
 * Author: Alessandro Thea
 *
 * Created on November 6, 2014, 4:50 PM
 */

#include "swatch/mp7/MP7TTCInterface.hpp"


// MP7 Headers
#include "mp7/MP7MiniController.hpp"
#include "mp7/CtrlNode.hpp"
#include "mp7/TTCNode.hpp"
#include "mp7/ReadoutNode.hpp"

namespace swatch {
namespace mp7 {


MP7TTCInterface::MP7TTCInterface(::mp7::MP7MiniController& controller) :
mDriver(controller),
mMetricBC0Counter(registerMetric<uint32_t>("bc0Counter")),
mMetricEC0Counter(registerMetric<uint32_t>("ec0Counter")),
mMetricOC0Counter(registerMetric<uint32_t>("oc0Counter")),
mMetricResyncCounter(registerMetric<uint32_t>("resyncCounter")),
mMetricStartCounter(registerMetric<uint32_t>("startCounter")),
mMetricStopCounter(registerMetric<uint32_t>("stopCounter")),
mMetricTestCounter(registerMetric<uint32_t>("testCounter")) {
}


MP7TTCInterface::~MP7TTCInterface() {
}


void
MP7TTCInterface::clearCounters() {
  mDriver.getTTC().clear();
}


void MP7TTCInterface::clearErrors() {
  mDriver.getTTC().clearErrors();
}


void MP7TTCInterface::retrieveMetricValues() {
  
  const ::mp7::TTCNode& ttc = mDriver.getTTC();
  const ::mp7::CtrlNode& ctrl = mDriver.getCtrl();

  // L1A Counter = Event Counter (next event)-1
  setMetricValue<>(mMetricL1ACounter, ttc.readEventCounter()-1);
  
  setMetricValue<>(mMetricBunchCounter, ttc.readBunchCounter());
  setMetricValue<>(mMetricOrbitCounter, ttc.readOrbitCounter());

  setMetricValue<>(mMetricSingleBitErrors, ttc.readSingleBitErrorCounter());
  setMetricValue<>(mMetricDoubleBitErrors, ttc.readDoubleBitErrorCounter());
  setMetricValue<>(mMetricIsClock40Locked, ctrl.clock40Locked());
  setMetricValue<>(mMetricHasClock40Stopped, !ctrl.clock40Locked());
  setMetricValue<>(mMetricIsBC0Locked, ttc.readBC0Locked());



  // <node id="cmd_ctrs" address="0x10" description="TTC command counters" fwinfo="endpoint;width=3">
  //   <node id="bc0_ctr" address="0x0"/>
  //   <node id="ec0_ctr" address="0x1"/>
  //   <node id="resync_ctr" address="0x2"/>
  //   <node id="oc0_ctr" address="0x3"/>
  //   <node id="test_sync_ctr" address="0x4"/>
  //   <node id="start_ctr" address="0x5"/>
  //   <node id="stop_ctr" address="0x6"/>

  uhal::ValWord<uint32_t> bc0Counter, ec0Counter, oc0Counter, resyncCounter, startCounter, stopCounter, testCounter;
  bc0Counter = ttc.getNode("cmd_ctrs.bc0_ctr").read();
  oc0Counter = ttc.getNode("cmd_ctrs.oc0_ctr").read();
  ec0Counter = ttc.getNode("cmd_ctrs.ec0_ctr").read();
  resyncCounter = ttc.getNode("cmd_ctrs.resync_ctr").read();
  startCounter = ttc.getNode("cmd_ctrs.start_ctr").read();
  stopCounter = ttc.getNode("cmd_ctrs.stop_ctr").read();
  testCounter = ttc.getNode("cmd_ctrs.test_sync_ctr").read();

  ttc.getClient().dispatch();

  setMetricValue<>(mMetricBC0Counter, (uint32_t)bc0Counter);
  setMetricValue<>(mMetricEC0Counter, (uint32_t)ec0Counter);
  setMetricValue<>(mMetricOC0Counter, (uint32_t)oc0Counter);
  setMetricValue<>(mMetricResyncCounter, (uint32_t)resyncCounter);
  setMetricValue<>(mMetricStartCounter, (uint32_t)startCounter);
  setMetricValue<>(mMetricStopCounter, (uint32_t)stopCounter);
  setMetricValue<>(mMetricTestCounter, (uint32_t)testCounter);

}

} // namespace mp7
} // namespace swatch
