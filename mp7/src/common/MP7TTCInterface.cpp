/* 
 * File:   MP7TCCInterface.hpp
 * Author: Alessandro Thea
 *
 * Created on November 6, 2014, 4:50 PM
 */

#include "swatch/mp7/MP7TTCInterface.hpp"


// MP7 Headers
#include "mp7/MP7Controller.hpp"
#include "mp7/CtrlNode.hpp"
#include "mp7/TTCNode.hpp"
#include "mp7/ReadoutNode.hpp"

namespace swatch {
namespace mp7 {


MP7TTCInterface::MP7TTCInterface(::mp7::MP7Controller& controller) :
mDriver(controller),
mBC0Counter(registerMetric<uint32_t>("bc0Counter")),
mEC0Counter(registerMetric<uint32_t>("ec0Counter")),
mOC0Counter(registerMetric<uint32_t>("oc0Counter")),
mResyncCounter(registerMetric<uint32_t>("resyncCounter")),
mStartCounter(registerMetric<uint32_t>("startCounter")),
mStopCounter(registerMetric<uint32_t>("stopCounter")),
mTestCounter(registerMetric<uint32_t>("testCounter")) {
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
  
  setMetricValue<>(metricBunchCounter_, ttc.readBunchCounter());
  setMetricValue<>(metricOrbitCounter_, ttc.readOrbitCounter());
  setMetricValue<>(metricEventCounter_, ttc.readEventCounter());

  setMetricValue<>(metricSingleBitErrors_, ttc.readSingleBitErrorCounter());
  setMetricValue<>(metricDoubleBitErrors_, ttc.readDoubleBitErrorCounter());
  setMetricValue<>(metricIsClock40Locked_, ctrl.clock40Locked());
  setMetricValue<>(metricHasClock40Stopped_, ctrl.clock40Stopped());
  setMetricValue<>(metricIsBC0Locked_, ttc.readBC0Locked());



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

  setMetricValue<>(mBC0Counter, (uint32_t)bc0Counter);
  setMetricValue<>(mEC0Counter, (uint32_t)ec0Counter);
  setMetricValue<>(mOC0Counter, (uint32_t)oc0Counter);
  setMetricValue<>(mResyncCounter, (uint32_t)resyncCounter);
  setMetricValue<>(mStartCounter, (uint32_t)startCounter);
  setMetricValue<>(mStopCounter, (uint32_t)stopCounter);
  setMetricValue<>(mTestCounter, (uint32_t)testCounter);

}

} // namespace mp7
} // namespace swatch
