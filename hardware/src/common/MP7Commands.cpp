/* 
 * @file    MP7Commands.cpp
 * @author  Alessandro Thea
 * @date    February 2015
 *
 */



#include "swatch/hardware/MP7Commands.hpp"

// XDAQ Headers
#include "xdata/Integer.h"

// Swatch Hardware Headers
#include "swatch/hardware/MP7Processor.hpp"

// MP7 Core Headers
#include "mp7/MP7Controller.hpp"
#include "mp7/PathConfigurator.hpp"

namespace swatch {
namespace hardware {


//---
MP7ResetCommand::MP7ResetCommand(core::ActionableObject* aHandler) : 
    Command(aHandler, xdata::Integer() ) {

    // Integer result?

}

//---
MP7ResetCommand::~MP7ResetCommand() {

}

//---
void MP7ResetCommand::code() {

  MP7Processor* p = getHandler<MP7Processor>();

  // TODO: acquire lock
  std::string mode = "external";
  p->driver().reset(mode, mode, mode);

  setDone("Reset completed");
}


//---
MP7ConfigureLoopback::MP7ConfigureLoopback(core::ActionableObject* aHandler) :
  Command(aHandler, xdata::Integer() ){

}


//---
MP7ConfigureLoopback::~MP7ConfigureLoopback() {

}


//---
void MP7ConfigureLoopback::code() {
  MP7Processor* p = getHandler<MP7Processor>();
  mp7::ChannelsManager mgr = p->driver().channelMgr();
  
  
  // TODO: acquire lock
  // Play patterns from zero
  mp7::orbit::Point zero;
  // Create a configurator to play patterns in loopback
  mp7::TestPathConfigurator txPattern(mp7::PathConfigurator::kPattern, zero, p->driver().getMetric());

  setProgress(0.1, "Configuring buffers");
  // Apply the configuration to TX buffers
  mgr.configureBuffers(mp7::kTxBuffer, txPattern);

  setProgress(0.2, "Configuring links");

  // Reset links (we're resetting both rx and tx. Should we be more selective in the future?)
  mgr.resetLinks();
  
  // Configure RX links in loopback, standard polarity
  mgr.configureLinks(true, false, false);
  
  // All done, clear the counters
  mgr.clearLinkCounters();

  setProgress(0.8, "Aligning links");

  // And then align them
  mgr.minimizeAndAlignLinks();
  
  setDone("All links aligned");
  
}


} /* namespace hardware */
} /* namespace swatch */