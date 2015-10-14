/**
 * @file    AMC13Service.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    07/11/14
 */

#include "swatch/amc13/AMC13Manager.hpp"


// Swatch Headers
#include "swatch/core/Factory.hpp"
#include "swatch/logger/Log.hpp"
#include "swatch/dtm/DaqTTCStub.hpp"
#include "swatch/core/CommandSequence.hpp"
#include "swatch/dtm/AMCPortCollection.hpp"


#include "swatch/amc13/AMC13Commands.hpp"
#include "swatch/amc13/TTCInterface.hpp"
#include "swatch/amc13/SLinkExpress.hpp"
#include "swatch/amc13/AMCPort.hpp"
#include "swatch/amc13/EVBInterface.hpp"

// XDAQ headers
#include "xdata/String.h"

// AMC13 Headers
#include "amc13/AMC13.hh"
#include "swatch/amc13/EVBInterface.hpp"

// Boost Headers
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <boost/preprocessor/facilities.hpp>


namespace swlo = swatch::logger;


SWATCH_REGISTER_CLASS(swatch::amc13::AMC13Manager)

namespace swatch {
namespace amc13 {


// --------------------------------------------------------
AMC13Manager::AMC13Manager(const swatch::core::AbstractStub& aStub) :
  swatch::dtm::DaqTTCManager(aStub),
  mDriver(0x0),
  mFwVersionT1(registerMetric<uint32_t>("fwVersionT1")),
  mFwVersionT2(registerMetric<uint32_t>("fwVersionT2")) {

  // Create driver first
  using ::amc13::AMC13;
  const dtm::DaqTTCStub& desc = getStub();

  uhal::HwInterface t1 = uhal::ConnectionManager::getDevice("T1", desc.uriT1, desc.addressTableT1);
  uhal::HwInterface t2 = uhal::ConnectionManager::getDevice("T2", desc.uriT2, desc.addressTableT2);

  mDriver = new AMC13(t1, t2);

  // Then Monitoring interfaces
  registerInterface(new TTCInterface(*mDriver));
  // Hard-coded id for the moment
  registerInterface(new SLinkExpress(0,*mDriver));
  
  registerInterface(new dtm::AMCPortCollection());

  BOOST_FOREACH( uint32_t s, getStub().amcSlots) {
    getAMCPorts().addPort(new AMCPort(s, *mDriver));
  }
  
  registerInterface(new EVBInterface(*mDriver));
  
  // Commands
  core::Command& coldResetCmd = registerFunctionoid<RebootCommand>("rebootCmd");
  core::Command& resetCmd = registerFunctionoid<ResetCommand>("resetCmd");
  core::Command& cfgTTCCmd = registerFunctionoid<ConfigureTTCCommand>("configTCCCmd");
  core::Command& cfgDAQCmd = registerFunctionoid<ConfigureDAQCommand>("configDAQCmd");
  core::Command& startCmd = registerFunctionoid<StartCommand>("startCmd");
  core::Command& stopCmd = registerFunctionoid<StopCommand>("stopCmd");

  // Sequences
  registerCommandSequence("configSeq", resetCmd).then(cfgTTCCmd).then(cfgDAQCmd);

  // State machine
  dtm::RunControlFSM& lFSM = getRunControlFSM();
  lFSM.coldReset.add(coldResetCmd);
  lFSM.clockSetup.add(resetCmd).add(cfgTTCCmd);
  lFSM.cfgDaq.add(cfgDAQCmd);
  lFSM.start.add(startCmd);
  lFSM.stopFromPaused.add(stopCmd);
  lFSM.stopFromRunning.add(stopCmd);


  uint32_t vT1 = mDriver->read(AMC13::T1, "STATUS.FIRMWARE_VERS");
  uint32_t vT2 = mDriver->read(AMC13::T2, "STATUS.FIRMWARE_VERS");
  LOG(swlo::kNotice) << "AMC13 Service '" << getId() << "' built. T1 ver: 0x" << std::hex << vT1 << " T2 ver: 0x" << std::hex << vT2;

}


// --------------------------------------------------------
AMC13Manager::~AMC13Manager() {
  delete mDriver;
}

// --------------------------------------------------------
void AMC13Manager::retrieveMetricValues() {
  using ::amc13::AMC13;

  setMetricValue<>(mFwVersionT1, mDriver->read(::amc13::AMC13::T1, "STATUS.FIRMWARE_VERS"));
  setMetricValue<>(mFwVersionT2, mDriver->read(::amc13::AMC13::T2, "STATUS.FIRMWARE_VERS"));

  setMetricValue<>(daqMetricFedId_, (uint16_t) mDriver->read(AMC13::T1, "CONF.ID.SOURCE_ID"));
}


} // namespace amc13
} // namespace swatch
