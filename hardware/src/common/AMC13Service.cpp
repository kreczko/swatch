/**
 * @file    AMC13Service.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    07/11/14
 */

#include "swatch/hardware/AMC13Service.hpp"

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/system/AMC13ServiceStub.hpp"
#include "swatch/system/ServiceFactory.hpp"
#include "swatch/hardware/AMC13Commands.hpp"

// XDAQ Headers
#include "xdata/String.h"

// AMC13 Headers
#include "amc13/AMC13.hh"

// Boost Headers
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

namespace swco = swatch::core;
namespace swlo = swatch::logger;
namespace swhw = swatch::hardware;

namespace swatch {
namespace hardware {
    
SWATCH_SERVICE_REGISTER_CLASS(AMC13Service)


//---
AMC13Service::AMC13Service(const std::string& aId, const core::XParameterSet& aPars) :
    swatch::system::DaqTTCService(aId, aPars),
    driver_(0x0) {
  
    registerCommand<AMC13ResetCommand>("reset");
    
    system::AMC13ServiceStub& desc = aPars.get<system::AMC13ServiceBag>("stub").bag;

    crate_ = desc.crate;
    slot_  = desc.slot;
    
    //TODO: Switch to HwInterface based constructor
//    driver_ = new amc13::AMC13(desc.uriT1, static_cast<std::string>(desc.addressTableT1).substr(7),
//                        desc.uriT2, static_cast<std::string>(desc.addressTableT2).substr(7) );

    uhal::HwInterface t1 = uhal::ConnectionManager::getDevice("T1", desc.uriT1, desc.addressTableT1);
    uhal::HwInterface t2 = uhal::ConnectionManager::getDevice("T2", desc.uriT2, desc.addressTableT2);

    driver_ = new amc13::AMC13(t1, t2);

    uint32_t vT1 = driver_->read(amc13::AMC13::T1, "STATUS.FIRMWARE_VERS");
    uint32_t vT2 = driver_->read(amc13::AMC13::T2, "STATUS.FIRMWARE_VERS");
    LOG(swlo::kNotice) << "AMC13 Service '" << id() << "' built. T1 ver: 0x" << std::hex << vT1 << " T2 ver: 0x" << std::hex << vT2;

}


//---
AMC13Service::~AMC13Service() {

}


//---
uint32_t
AMC13Service::getSlot() const {
    return slot_;
}


//---
const std::string&
AMC13Service::getCrateId() const {
    return crate_;
}


//---
void
AMC13Service::enableTTC(const std::vector<uint32_t>& slots) {
    uint32_t mask(0x0);
    BOOST_FOREACH( uint32_t s, slots ) {
        mask |= 1 << (s-1);
    }
    LOG(swlo::kDebug) << "AMC mask: 0x" << std::hex <<  mask;
    // apply mask
    driver_->AMCInputEnable(mask);

}

//---
void AMC13Service::reset() {

  // Stopping the run sounds like a good idea
  driver_->endRun();
  
  // Reset T2 first
  driver_->reset(amc13::AMC13::T2);
  
  // Disable all AMC13 links
  driver_->AMCInputEnable(0x0);

  // Clear the TTS inputs mask
  driver_->write(amc13::AMC13Simple::T1, "CONF.AMC.TTS_DISABLE_MASK", 0x0);

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
  driver_->reset(amc13::AMC13::T1);

  // Just in case  
  driver_->resetDAQ();

  // Necessary?
  driver_->resetCounters();
  
  // TODO: Do we want this here? Doesn't sound like a bad idea.
  // Enable TTC all
  driver_->enableAllTTC();
}


void
AMC13Service::configureClock(const std::string& mode) {
    
  LOG(swlo::kInfo) << "step 0: BCNT_ERROR = " << driver_->read(amc13::AMC13::T2,"STATUS.TTC.BCNT_ERROR"); ;
  if ( mode == "external" ) {
    driver_->reset(amc13::AMC13Simple::T1);
    driver_->reset(amc13::AMC13Simple::T2);
    LOG(swlo::kDebug) << "Enabling external TTC/BC0 inputs";
    driver_->localTtcSignalEnable(false);
    
  } else if ( mode == "ttsloopback" ) {
    driver_->reset(amc13::AMC13Simple::T1);
    driver_->reset(amc13::AMC13Simple::T2);
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
  driver_->reset(amc13::AMC13::T2);

  
  uint32_t clkfreq  = driver_->read(amc13::AMC13::T2,"STATUS.TTC.CLK_FREQ");
  uint32_t bc0count = driver_->read(amc13::AMC13::T2,"STATUS.TTC.BC0_COUNTER");
  uint32_t bcnt_err = driver_->read(amc13::AMC13::T2,"STATUS.TTC.BCNT_ERROR");
  uint32_t sbit_err = driver_->read(amc13::AMC13::T2,"STATUS.TTC.SBIT_ERROR");
  uint32_t mbit_err = driver_->read(amc13::AMC13::T2,"STATUS.TTC.MBIT_ERROR");
  
  LOG(swlo::kInfo) << " * Clk40 Freq: " << (double)clkfreq*50 << " Hz";
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


} // namespace hardware
} // namespace swatch