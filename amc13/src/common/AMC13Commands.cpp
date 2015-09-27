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
#include "xdata/UnsignedInteger.h"

// Swatch Headers
#include "swatch/amc13/AMC13Manager.hpp"
#include "swatch/logger/Log.hpp"

// AMC13 Headers
#include "amc13/AMC13.hh"


namespace swlo = swatch::logger;

namespace swatch {
namespace amc13 {

//---
RebootCommand::RebootCommand(const std::string& aId) : 
  Command(aId, xdata::Boolean(false)) {
}

//---
RebootCommand::~RebootCommand() {
}


//---
core::Command::State RebootCommand::code(const core::XParameterSet& params) {
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
ResetCommand::ResetCommand(const std::string& aId) :
  Command(aId, xdata::Integer()) {
 
  // TTC orbit counter encoding
  registerParameter("resyncCmd", xdata::UnsignedInteger(0x4));
  // TTC Resync encoding
  registerParameter("ocrCmd", xdata::UnsignedInteger(0x8));
  // Bunch counter reset offset
  registerParameter("bcnOffset", xdata::UnsignedInteger(0xdec-24));

}


//---
ResetCommand::~ResetCommand() {

}


//---
core::Command::State ResetCommand::code(const core::XParameterSet& params) {

    AMC13Manager* amc13mgr = getParent<AMC13Manager>();
   
    uint32_t resyncCmd = params.get<xdata::UnsignedInteger>("resyncCmd").value_;
    uint32_t ocrCmd    = params.get<xdata::UnsignedInteger>("ocrCmd").value_;
    uint32_t bcnOffset = params.get<xdata::UnsignedInteger>("bcnOffset").value_;

    ::amc13::AMC13& board = amc13mgr->driver();
    
    // Reset T1 chip
    board.reset(::amc13::AMC13Simple::T1);

    // Reset T2 chip
    board.reset(::amc13::AMC13Simple::T2);
    
    // Take the board out of run mode
    board.endRun();

    // Disable SFP outputs
    board.sfpOutputEnable(0);
    
    // Disable AMC13-AMC connection
    board.AMCInputEnable(0);
    
    // Bunch counter offset
    board.setBcnOffset(bcnOffset);

    // configure TTC commands
    board.setOcrCommand(ocrCmd);

    // Replace with python bindings when they come out...
    board.write(::amc13::AMC13Simple::T1,"CONF.TTC.RESYNC.COMMAND",resyncCmd); 
    
    // Disable any TTS mask
    board.ttsDisableMask(0x0);
    
    // Disable local triggers
    board.localTtcSignalEnable(false);
    
    // Disable monitor buffer backpressure
    board.monBufBackPressEnable(false);

    // activate TTC output to all AMCs
    board.enableAllTTC();

    return kDone;
}

// --------------------------------------------------------
ConfigureCommand::ConfigureCommand(const std::string& aId) :
core::Command(aId, xdata::Integer()) {
  // slots, fedId, slink, localTtc=False
  // Fed id
  registerParameter("fedId", xdata::UnsignedInteger(0xfff));
  // Slinks
  registerParameter("slinkMask", xdata::UnsignedInteger(0x0));
  // Local ttc configuration
  registerParameter("localTTC", xdata::Boolean(false));
}

// --------------------------------------------------------
ConfigureCommand::~ConfigureCommand() {

}

core::Command::State 
ConfigureCommand::code(const core::XParameterSet& params) {

  uint32_t fedId = params.get<xdata::UnsignedInteger>("fedId").value_;
  uint32_t slinkMask = params.get<xdata::UnsignedInteger>("slinkMask").value_;
  uint32_t localTTC = params.get<xdata::UnsignedInteger>("localTTC").value_;
  
  AMC13Manager* amc13mgr = getParent<AMC13Manager>();

  ::amc13::AMC13& board = amc13mgr->driver();

  // TODO: replace with a proper parameters
  uint32_t bitmask = 0x0;
  board.AMCInputEnable(bitmask);
  
  // FIXME: Take fedId from stub?
  // Set FED ID
  board.setFEDid(fedId);
  
  // Enable SFPs
  board.sfpOutputEnable(slinkMask);
  
  // Enable daq link if any of the SFPs is enabled
  board.daqLinkEnable( slinkMask != 0);
  
  // Reset counters
  board.resetCounters();

  // Reset T1, just in case
  board.reset(::amc13::AMC13Simple::T1);

  // Do we need this in this command?
  if ( localTTC ) board.localTtcSignalEnable(true);

  return kDone;
}

StartCommand::StartCommand(const std::string& aId) :
core::Command(aId, xdata::Integer()) {
}

StartCommand::~StartCommand() {
}

core::Command::State
StartCommand::code(const core::XParameterSet& params) {
  getParent<AMC13Manager>()->driver().startRun();
  
  return kDone;
}

StopCommand::StopCommand(const std::string& aId) :
core::Command(aId, xdata::Integer()) {

}

StopCommand::~StopCommand() {

}

core::Command::State
StopCommand::code(const core::XParameterSet& params) {
  getParent<AMC13Manager>()->driver().endRun();

  return kDone;
}



} // namespace amc13
} // namespace swatch
