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

// Boost Headers
#include <boost/foreach.hpp>


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
      return State::kDone;
    }
    catch (const uhal::exception::exception& e){
    }
    boost::this_thread::sleep_for( boost::chrono::seconds(1));
  }
  
  setStatusMsg("AMC13 did not wake up after reboot");
  return State::kError;
}


//---
ResetCommand::ResetCommand(const std::string& aId) :
  Command(aId, xdata::Integer()) {
}


//---
ResetCommand::~ResetCommand() {

}


//---
core::Command::State ResetCommand::code(const core::XParameterSet& params) {

    AMC13Manager* amc13mgr = getParent<AMC13Manager>();
   
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
        
    // Disable any TTS mask
    board.ttsDisableMask(0x0);

    // Disable local triggers
    board.localTtcSignalEnable(false);
    
    // Disable fake data generation
    board.fakeDataEnable(false);
    
    // Disable monitor buffer backpressure
    board.monBufBackPressEnable(false);

    // activate TTC output to all AMCs
    board.enableAllTTC();

    return State::kDone;
}


//---
ConfigureTTCCommand::ConfigureTTCCommand(const std::string& aId) :
  Command(aId, xdata::Integer()) {
 
  // TTC orbit counter encoding
  registerParameter("resyncCmd", xdata::UnsignedInteger(0x4));
  // TTC Resync encoding
  registerParameter("ocrCmd", xdata::UnsignedInteger(0x8));
  // Local ttc configuration
  registerParameter("localTTC", xdata::Boolean(false));
}


//---
ConfigureTTCCommand::~ConfigureTTCCommand() {
}


//---
core::Command::State
ConfigureTTCCommand::code(const core::XParameterSet& params) {

    AMC13Manager* amc13mgr = getParent<AMC13Manager>();
   
    uint32_t resyncCmd = params.get<xdata::UnsignedInteger>("resyncCmd").value_;
    uint32_t ocrCmd    = params.get<xdata::UnsignedInteger>("ocrCmd").value_;
    uint32_t localTTC = params.get<xdata::Boolean>("localTTC").value_;

    ::amc13::AMC13& board = amc13mgr->driver();
    
    // configure TTC commands
    board.setOcrCommand(ocrCmd);
    board.write(::amc13::AMC13Simple::T1,"CONF.TTC.OCR_MASK",0x0);

    // Replace with python bindings when they come out...
    board.write(::amc13::AMC13Simple::T1,"CONF.TTC.RESYNC.COMMAND",resyncCmd);
    board.write(::amc13::AMC13Simple::T1,"CONF.TTC.RESYNC.MASK",0x0);
    
    // Do we need this in this command?
    if ( localTTC ) board.localTtcSignalEnable(true);
  
    // activate TTC output to all AMCs
    board.enableAllTTC();

    return State::kDone;
}


// --------------------------------------------------------
ConfigureDAQCommand::ConfigureDAQCommand(const std::string& aId) :
core::Command(aId, xdata::Integer()) {
  // slots, fedId, slink, localTtc=False
  // Slinks
  registerParameter("slinkMask", xdata::UnsignedInteger(0x0));
  // Bunch counter reset offset
  registerParameter("bcnOffset", xdata::UnsignedInteger(0xdec-24));
}

// --------------------------------------------------------
ConfigureDAQCommand::~ConfigureDAQCommand() {

}

// --------------------------------------------------------
core::Command::State 
ConfigureDAQCommand::code(const core::XParameterSet& params) {

  uint32_t slinkMask = params.get<xdata::UnsignedInteger>("slinkMask").value_;
  uint32_t bcnOffset = params.get<xdata::UnsignedInteger>("bcnOffset").value_;
  
  AMC13Manager* amc13mgr = getParent<AMC13Manager>();

  ::amc13::AMC13& board = amc13mgr->driver();

  // TODO: replace with a proper parameters
  uint32_t bitmask = 0x0;
  BOOST_FOREACH( uint32_t s, amc13mgr->getStub().amcSlots) {
    bitmask |= ( 1<< (s-1) );
  }
  board.AMCInputEnable(bitmask);
  
  // Set FED ID
  board.setFEDid(amc13mgr->getStub().fedId);
  
  // Enable SFPs
  board.sfpOutputEnable(slinkMask);
  
  // Enable daq link if any of the SFPs is enabled
  board.daqLinkEnable( slinkMask != 0);
  
  // Bunch counter offset
  board.setBcnOffset(bcnOffset);

  // Reset counters
  board.resetCounters();

  // Reset T1, just in case
  board.reset(::amc13::AMC13Simple::T1);

  return State::kDone;
}


// --------------------------------------------------------
StartCommand::StartCommand(const std::string& aId) :
core::Command(aId, xdata::Integer()) {
}


// --------------------------------------------------------
StartCommand::~StartCommand() {
}


// --------------------------------------------------------
core::Command::State
StartCommand::code(const core::XParameterSet& params) {
  getParent<AMC13Manager>()->driver().startRun();
  
  return State::kDone;
}


// --------------------------------------------------------
StopCommand::StopCommand(const std::string& aId) :
core::Command(aId, xdata::Integer()) {

}

// --------------------------------------------------------
StopCommand::~StopCommand() {

}


// --------------------------------------------------------
core::Command::State
StopCommand::code(const core::XParameterSet& params) {
  getParent<AMC13Manager>()->driver().endRun();

  return State::kDone;
}



} // namespace amc13
} // namespace swatch
