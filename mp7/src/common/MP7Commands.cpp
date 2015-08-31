/* 
 * @file    MP7Commands.cpp
 * @author  Alessandro Thea
 * @date    February 2015
 *
 */

#include "swatch/mp7/MP7Commands.hpp"


// XDAQ Headers
#include "xdata/Boolean.h"
#include "xdata/Integer.h"
#include "xdata/UnsignedInteger.h"
#include "xdata/String.h"

// Swatch headers
#include "swatch/mp7/MP7Processor.hpp"

// MP7 Core Headers
#include "mp7/MP7Controller.hpp"
#include "mp7/PathConfigurator.hpp"
#include "mp7/exception.hpp"


namespace swatch {
namespace mp7 {


//---
MP7ResetCommand::MP7ResetCommand(const std::string& aId) : 
    Command(aId, xdata::Integer() ) {

    // Integer result?
    registerParameter("clockSource",xdata::String("external"));

}

//---
MP7ResetCommand::~MP7ResetCommand() {

}

//---
core::Command::State MP7ResetCommand::code(const core::XParameterSet& params) {

  MP7Processor* p = getParent<MP7Processor>();
  setProgress(0.,"Resetting");

  std::string mode = params.get<xdata::String>("clockSource").value_;
  try {
  // TODO: acquire lock
    // std::string mode = "external";
    p->driver().reset(mode, mode, mode);
  } catch ( ::mp7::exception &e ) {
    
    setStatusMsg("Reset failed: "+e.description());
    return kError;
  }
  setStatusMsg("Reset completed");
  return kDone;
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
core::Command::State MP7SetupLinks::code(const core::XParameterSet& params) {
  
  MP7Processor* p = getParent<MP7Processor>();
  ::mp7::ChannelsManager mgr = p->driver().channelMgr();

  setProgress(0.,"Configuring links");

//  p->driver().configureLinks(loopback, orbittag, invpol)
  bool loopback = params.get<xdata::Boolean>("loopback").value_;
  bool orbittag = params.get<xdata::Boolean>("orbitTag").value_;
  bool invPol = params.get<xdata::Boolean>("invertPolarity").value_;

  if ( loopback ) {
    ::mp7::orbit::Point zero;
    // Create a configurator to play patterns in loopback
    ::mp7::TestPathConfigurator txPattern(::mp7::PathConfigurator::kPattern, zero, p->driver().getMetric());

    // Apply the configuration to TX buffers
    mgr.configureBuffers(::mp7::kTxBuffer, txPattern);
  
    setProgress(0.5, "Buffers configured");
  }
  
  mgr.configureLinks( loopback, orbittag, invPol);

  setProgress(0.9, "Clearing counters");
  
  // All done, clear the counters
  mgr.clearLinkCounters();
  setStatusMsg("Links configured");
  return kDone;
}


//---
MP7AlignLinks::MP7AlignLinks(const std::string& aId) :
  Command(aId, xdata::Integer()) {

  registerParameter("autoAlign",xdata::Boolean(true));
  registerParameter("alignBx",xdata::UnsignedInteger());
  registerParameter("alignCycle",xdata::UnsignedInteger());

}

//---
MP7AlignLinks::~MP7AlignLinks() {
}

//---
core::Command::State MP7AlignLinks::code(const core::XParameterSet& params) {

  MP7Processor* p = getParent<MP7Processor>();
  ::mp7::ChannelsManager mgr = p->driver().channelMgr();

  
  bool autoAlign = params.get<xdata::Boolean>("autoAlign").value_;
  const xdata::UnsignedInteger& alignBx = params.get<xdata::UnsignedInteger>("alignBx");
  const xdata::UnsignedInteger& alignCycle = params.get<xdata::UnsignedInteger>("alignCycle");
  
  if ( !autoAlign ) {
    if ( alignBx.isNaN() or not alignBx.isFinite()) {
      setStatusMsg("Aaargh!");
      return kError;
    }
  }
  
  setProgress(0.1, "Input parameters check completed");
  
  if ( autoAlign ) {
    setProgress(0.2, "Running auto-alignment");

    mgr.minimizeAndAlignLinks(3);
    auto ap = mgr.getAlignmentPositions();
    setStatusMsg("Links aligned somewhere" );
    return kDone;
  } else {
    setProgress(0.2, "Aligning links to " + boost::lexical_cast<std::string>(alignBx.value_) +"/"+ boost::lexical_cast<std::string>(alignCycle.value_));

    ::mp7::orbit::Point p( alignBx.value_, alignCycle.value_ );
    mgr.alignLinks(p);
    
    setStatusMsg("Links aligned to " + boost::lexical_cast<std::string>(alignBx.value_) +"/"+ boost::lexical_cast<std::string>(alignCycle.value_));
    return kDone;
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
core::Command::State MP7ConfigureLoopback::code(const core::XParameterSet& params) {
  MP7Processor* p = getParent<MP7Processor>();
  ::mp7::ChannelsManager mgr = p->driver().channelMgr();
  
  
  // TODO: acquire lock
  // Play patterns from zero
  ::mp7::orbit::Point zero;
  // Create a configurator to play patterns in loopback
  ::mp7::TestPathConfigurator txPattern(::mp7::PathConfigurator::kPattern, zero, p->driver().getMetric());

  setProgress(0.1, "Configuring buffers");
  // Apply the configuration to TX buffers
  mgr.configureBuffers(::mp7::kTxBuffer, txPattern);

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
  
  setStatusMsg("All links aligned");
  return kDone;
}


} /* namespace mp7 */
} /* namespace swatch */
