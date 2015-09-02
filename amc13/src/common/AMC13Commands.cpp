/**
 * @file    AMC13Commands.cpp
 * @author  Alessandro Thea
 * @date    February 2015
 *
 */

#include "swatch/amc13/AMC13Commands.hpp"


// XDAQ Headers
#include "xdata/Boolean.h"
#include "xdata/Integer.h"
#include "xdata/String.h"

// Swatch Headers
#include "swatch/amc13/AMC13Manager.hpp"
#include "swatch/logger/Log.hpp"

// AMC13 Headers
#include "amc13/AMC13.hh"


namespace swlo = swatch::logger;

namespace swatch {
namespace amc13 {

//---
AMC13RebootCommand::AMC13RebootCommand(const std::string& aId) : 
  Command(aId, xdata::Boolean(false)) {
}
  

//---
AMC13RebootCommand::~AMC13RebootCommand() {
}


//---
core::Command::State AMC13RebootCommand::code(const core::XParameterSet& params) {
  AMC13Manager* amc13 = getParent<AMC13Manager>();
  
  setStatusMsg("Loading FW from flash");
  
  ::amc13::AMC13& driver = amc13->driver();
  
  driver.getFlash()->loadFlash();
  
  setProgress(0.5, "Waiting for T1 & 2 to wake up again");

  
  // Sleep for 2 seconds first ...
  boost::this_thread::sleep_for( boost::chrono::seconds(2) );
  
  for(size_t i=1; i<=15; i++)
  {
    try {
      uint32_t vT1 = driver.read(::amc13::AMC13::T1, "STATUS.FIRMWARE_VERS");
      uint32_t vT2 = driver.read(::amc13::AMC13::T2, "STATUS.FIRMWARE_VERS");
      
      std::ostringstream oss;
      oss << "Firmware versions - T1: 0x" << std::hex << vT1 << ", T2: 0x" << vT2;
      setResult(xdata::Boolean(true));
      setStatusMsg("AMC13 is alive! " + oss.str());
      return kDone;
    }
    catch (const uhal::exception::exception& e){
    }
    boost::this_thread::sleep_for( boost::chrono::seconds(1));
  }
  
  setStatusMsg("AMC13 did not wake up after reboot");
  return kError;
}


//---
AMC13ResetCommand::AMC13ResetCommand(const std::string& aId) :
  Command(aId, xdata::Integer()) {
 
  registerParameter("mode", xdata::String());
}


//---
AMC13ResetCommand::~AMC13ResetCommand() {

}


//---
core::Command::State AMC13ResetCommand::code(const core::XParameterSet& params) {

    AMC13Manager* amc13 = getParent<AMC13Manager>();
   
    std::string mode = params.get<xdata::String>("mode").value_;

    using ::amc13::AMC13Simple;
  
    if ( mode == "ttsloopback" ) {
        amc13->driver().reset(AMC13Simple::T1);
        amc13->driver().reset(AMC13Simple::T2);
        LOG(swlo::kInfo) << "Enabling local TTC";
        amc13->driver().localTtcSignalEnable(true);
        
    } else if ( mode == "external" ) {
        amc13->driver().reset(AMC13Simple::T1);
        amc13->driver().reset(AMC13Simple::T2);
        LOG(swlo::kInfo) << "Enabling external TTC input" << std::endl;
        amc13->driver().localTtcSignalEnable(false);
    } else {
      std::ostringstream oss;
        oss << "Unknown option " << mode;
        LOG(swlo::kError) << oss.str();
        setStatusMsg(oss.str());
        return kError;
    }
    
    // and check the status
    // amc13->driver()->getStatus()->Report(1);
    
    return kDone;
}


} // namespace amc13
} // namespace swatch
