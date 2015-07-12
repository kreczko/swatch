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
#include "swatch/system/DaqTTCStub.hpp"
#include "swatch/amc13/AMC13Commands.hpp"
#include "swatch/amc13/AMC13Operations.hpp"

// XDAQ headers
#include "xdata/String.h"

// AMC13 Headers
#include "amc13/AMC13.hh"

// Boost Headers
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <boost/preprocessor/facilities.hpp>


namespace swlo = swatch::logger;


SWATCH_REGISTER_CLASS(swatch::amc13::AMC13Manager)

namespace swatch {
namespace amc13 {

using ::amc13::AMC13;

using ::amc13::AMC13Simple;


//---
AMC13Manager::AMC13Manager(const swatch::core::AbstractStub& aStub) :
    swatch::system::DaqTTCManager(aStub),
    driver_(0x0) {
  
    Register<AMC13ResetCommand>("reset");
    Register<AMC13Configure>("configure");

    const system::DaqTTCStub& desc = getStub();

    uhal::HwInterface t1 = uhal::ConnectionManager::getDevice("T1", desc.uriT1, desc.addressTableT1);
    uhal::HwInterface t2 = uhal::ConnectionManager::getDevice("T2", desc.uriT2, desc.addressTableT2);

    driver_ = new AMC13(t1, t2);

    uint32_t vT1 = driver_->read(AMC13::T1, "STATUS.FIRMWARE_VERS");
    uint32_t vT2 = driver_->read(AMC13::T2, "STATUS.FIRMWARE_VERS");
    LOG(swlo::kNotice) << "AMC13 Service '" << id() << "' built. T1 ver: 0x" << std::hex << vT1 << " T2 ver: 0x" << std::hex << vT2;

}


//---
AMC13Manager::~AMC13Manager() {
  delete driver_;
}


//---
void
AMC13Manager::enableTTC(const std::vector<uint32_t>& slots) {
    uint32_t mask(0x0);
    BOOST_FOREACH( uint32_t s, slots ) {
        mask |= 1 << (s-1);
    }
    LOG(swlo::kDebug) << "AMC mask: 0x" << std::hex <<  mask;
    // apply mask
    driver_->AMCInputEnable(mask);

}


//---
void AMC13Manager::reset() {

  // Stopping the run sounds like a good idea
  driver_->endRun();
  
  // Reset T2 first
  driver_->reset(AMC13::T2);
  
  // Disable all AMC13 links
  driver_->AMCInputEnable(0x0);

  // Clear the TTS inputs mask
  driver_->write(AMC13Simple::T1, "CONF.AMC.TTS_DISABLE_MASK", 0x0);

  // Disable fake data generator
  driver_->fakeDataEnable(false);
  
  // Disable local TTC signal
  driver_->localTtcSignalEnable(false);
  
  // Don't know what this does
  driver_->monBufBackPressEnable(false);

  // No prescales
  driver_->configurePrescale(0,1);
  
  // No internal L1A
  driver_->configureLocalL1A(false,0,1,1,0);
  
  // FED id 0 is a nice default
  driver_->setFEDid(0);
  
  // Then move onto T1
  driver_->reset(AMC13::T1);

  // Just in case  
  driver_->resetDAQ();

  // Necessary?
  driver_->resetCounters();
  
  // TODO: Do we want this here? Doesn't sound like a bad idea.
  // Enable TTC all
  driver_->enableAllTTC();
}


void
AMC13Manager::configureClock(const std::string& mode) {
    
  LOG(swlo::kInfo) << "step 0: BCNT_ERROR = " << driver_->read(AMC13::T2,"STATUS.TTC.BCNT_ERROR"); ;
  if ( mode == "external" ) {
    driver_->reset(AMC13Simple::T1);
    driver_->reset(AMC13Simple::T2);
    LOG(swlo::kDebug) << "Enabling external TTC/BC0 inputs";
    driver_->localTtcSignalEnable(false);
    
  } else if ( mode == "ttsloopback" ) {
    driver_->reset(AMC13Simple::T1);
    driver_->reset(AMC13Simple::T2);
    LOG(swlo::kDebug) << "Enabling local TTC";
    driver_->localTtcSignalEnable(true);
        
  } else {
    std::ostringstream oss;
    oss << "Unknown option " << mode;
    LOG(swlo::kError) << oss.str();
    throw std::runtime_error(oss.str());
  }
    
  // Tke a little nap waiting for the loopback to appear
  sleep(1);  

  // Then reset T2 again
  driver_->reset(AMC13::T2);

  
  uint32_t clkfreq  = driver_->read(AMC13::T2,"STATUS.TTC.CLK_FREQ");
  uint32_t bc0count = driver_->read(AMC13::T2,"STATUS.TTC.BC0_COUNTER");
  uint32_t bcnt_err = driver_->read(AMC13::T2,"STATUS.TTC.BCNT_ERROR");
  uint32_t sbit_err = driver_->read(AMC13::T2,"STATUS.TTC.SBIT_ERROR");
  uint32_t mbit_err = driver_->read(AMC13::T2,"STATUS.TTC.MBIT_ERROR");
  

  LOG(swlo::kInfo) << "AMC13 TTC Status";
  LOG(swlo::kInfo) << " * Clk40 frequency: " << (double)clkfreq*50 << " Hz";
  LOG(swlo::kInfo) << " * BC0 counter: " << bc0count;
  LOG(swlo::kInfo) << " * BC errors: " << bcnt_err;
  LOG(swlo::kInfo) << " * Single Bit errors: " << sbit_err;
  LOG(swlo::kInfo) << " * Double Bit errors: " << mbit_err;

  if ( bc0count == 0 or bcnt_err != 0) {
      throw std::runtime_error("AMC13 Clock error BC0 not locked/in error");    
  }

  if ( sbit_err != 0 or mbit_err != 0 ) {
      throw std::runtime_error("TTC Bit errors detected!");
  }
  
}

/*
double AMC13Manager::ttcClockFreq() const {
  uint32_t regValue = driver_->read(amc13::AMC13::T2,"STATUS.TTC.CLK_FREQ");
  return ((double)regValue*50);
}

uint32_t AMC13Manager::ttcBC0Counter() const {
  return driver_->read(amc13::AMC13::T2,"STATUS.TTC.BC0_COUNTER");
}
    
uint32_t AMC13Manager::ttcBC0Errors() const {
  return driver_->read(amc13::AMC13::T2,"STATUS.TTC.BCNT_ERROR");
}
    
uint32_t AMC13Manager::ttcSingleBitErrors() const {
  return driver_->read(amc13::AMC13::T2,"STATUS.TTC.SBIT_ERROR");
}
    
uint32_t AMC13Manager::ttcDoubleBitErrors() const {
  return driver_->read(amc13::AMC13::T2,"STATUS.TTC.MBIT_ERROR");
}
*/

void AMC13Manager::implementUpdateMetrics()
{
  setMetricValue<>(ttcMetricClockFreq_, (double) driver_->read(AMC13::T2,"STATUS.TTC.CLK_FREQ")*50 ); 
  setMetricValue<>(ttcMetricBC0Counter_, driver_->read(AMC13::T2,"STATUS.TTC.BC0_COUNTER"));
  setMetricValue<>(ttcMetricBC0Errors_, driver_->read(AMC13::T2,"STATUS.TTC.BCNT_ERROR"));
  setMetricValue<>(ttcMetricSingleBitErrors_, driver_->read(AMC13::T2,"STATUS.TTC.SBIT_ERROR"));
  setMetricValue<>(ttcMetricDoubleBitErrors_, driver_->read(AMC13::T2,"STATUS.TTC.MBIT_ERROR"));
  setMetricValue<>(daqMetricFedId_, (uint16_t) driver_->read(AMC13::T1,"CONF.ID.FED_ID"));
}


} // namespace amc13
} // namespace swatch
