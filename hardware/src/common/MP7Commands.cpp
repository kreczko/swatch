/* 
 * @file    MP7Commands.cpp
 * @author  Alessandro Thea
 * @date    February 2015
 *
 */

#include <xdata/Boolean.h>
#include <xdata/UnsignedInteger.h>

#include "swatch/hardware/MP7Commands.hpp"

// XDAQ Headers
#include "xdata/Integer.h"

// Swatch headers
#include "swatch/hardware/MP7Processor.hpp"

// MP7 Core Headers
#include "mp7/MP7Controller.hpp"
#include "mp7/PathConfigurator.hpp"

namespace swatch {
namespace hardware {


//---
MP7ResetCommand::MP7ResetCommand(const std::string& aId) : 
    Command(aId, xdata::Integer() ) {

    // Integer result?

}

//---
MP7ResetCommand::~MP7ResetCommand() {

}

//---
void MP7ResetCommand::code(core::XParameterSet& params) {

  MP7Processor* p = getParent<MP7Processor>();
  setProgress(0.,"Resetting");

  // TODO: acquire lock
  std::string mode = "external";
  p->driver().reset(mode, mode, mode);

  setDone("Reset completed");
}


//---
MP7SetupLinks::MP7SetupLinks(const std::string& aId) :
  Command(aId, xdata::Integer()) {
  
  registerParameter("loopback",xdata::Boolean(false));
  registerParameter("orbitTag",xdata::Boolean(false));
  registerParameter("invertPolarity",xdata::Boolean(false));

}


//---
MP7SetupLinks::~MP7SetupLinks() {
}


//---
void MP7SetupLinks::code(core::XParameterSet& params) {
  
  MP7Processor* p = getParent<MP7Processor>();
  mp7::ChannelsManager mgr = p->driver().channelMgr();

  setProgress(0.,"Configuring links");

//  p->driver().configureLinks(loopback, orbittag, invpol)
  bool loopback = params.get<xdata::Boolean>("loopback");
  bool orbittag = params.get<xdata::Boolean>("orbitTag");
  bool invPol = params.get<xdata::Boolean>("invertPolarity");

  if ( loopback ) {
    mp7::orbit::Point zero;
    // Create a configurator to play patterns in loopback
    mp7::TestPathConfigurator txPattern(mp7::PathConfigurator::kPattern, zero, p->driver().getMetric());

    // Apply the configuration to TX buffers
    mgr.configureBuffers(mp7::kTxBuffer, txPattern);
  
    setProgress(0.5, "Buffers configured");
  }
  
  mgr.configureLinks( loopback, orbittag, invPol);

  setProgress(0.9, "Clearing counters");
  
  // All done, clear the counters
  mgr.clearLinkCounters();
  setDone("Links configured");
}


//---
MP7AlignLinks::MP7AlignLinks(const std::string& aId) :
  Command(aId, xdata::Integer()) {

  registerParameter("autoAlign",xdata::Boolean(true));
  registerParameter("alignBx",xdata::UnsignedInteger());

}

//---
MP7AlignLinks::~MP7AlignLinks() {
}

//---
void MP7AlignLinks::code(core::XParameterSet& params) {

  MP7Processor* p = getParent<MP7Processor>();
  mp7::ChannelsManager mgr = p->driver().channelMgr();

  
  xdata::Boolean& autoAlign = params.get<xdata::Boolean>("autoAlign");
  xdata::UnsignedInteger& alignBx = params.get<xdata::UnsignedInteger>("alignBx");
  
  if ( !autoAlign ) {
    if ( alignBx.isNaN() or not alignBx.isFinite()) {
      setError("Aaargh!");
    }
  }
  
  setProgress(0.1, "Input parameters check completed");
  
  if ( autoAlign ) {
    setProgress(0.2, "Running auto-alignment");

    mgr.minimizeAndAlignLinks(3);
    auto ap = mgr.getAlignmentPositions();
    setDone("Links aligned somewhere" );

  } else {
    setProgress(0.2, "Aligning links to " + alignBx.toString());

    mp7::orbit::Point p( alignBx );
    mgr.alignLinks(p);
    
    setDone("Links aligned to " + alignBx.toString() );

  }
  
  
  
}



//---
MP7ConfigureLoopback::MP7ConfigureLoopback(const std::string& aId) :
  Command(aId, xdata::Integer() ){
}


//---
MP7ConfigureLoopback::~MP7ConfigureLoopback() {

}


//---
void MP7ConfigureLoopback::code(core::XParameterSet& params) {
  MP7Processor* p = getParent<MP7Processor>();
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
