
#include "swatch/mp7/ResetCommand.hpp"


#include "xdata/String.h"
#include "xdata/Integer.h"

#include "mp7/MP7Controller.hpp"
#include "mp7/PathConfigurator.hpp"
#include "mp7/TTCNode.hpp"

#include "swatch/mp7/MP7Processor.hpp"


namespace swatch {
namespace mp7 {


ResetCommand::ResetCommand(const std::string& aId):
  Command(aId, xdata::Integer()) {
  registerParameter("clockSource", xdata::String("external"));
  registerParameter("clockConfig", xdata::String(""));
  registerParameter("ttcConfig", xdata::String(""));
}
    
ResetCommand::~ResetCommand(){
}
    
::swatch::core::Command::State ResetCommand::code(const ::swatch::core::XParameterSet& params)
{
     
  // setProgress(0., "Resetting MP7");
  
  swatch::mp7::MP7Processor* p = getParent<swatch::mp7::MP7Processor>();
  ::mp7::MP7Controller& driver = p->driver();
  setProgress(0.,"Resetting clocks");

  std::string mode = params.get<xdata::String>("clockSource").value_;
  std::string clkCfg = params.get<xdata::String>("clockConfig").value_;
  std::string ttcCfg = params.get<xdata::String>("ttcConfig").value_;

  if ( mode.compare("internal") && mode.compare("external") ) {
      setStatusMsg("Clock can be set only to 'internal' or 'external'");
      return kError;
  }

  try {
  // TODO: acquire lock
    // std::string mode = "external";
    driver.reset(mode, clkCfg, ttcCfg);
  } catch ( ::mp7::exception &e ) {
    
    setStatusMsg("Reset failed: "+e.description());
    return kError;
  }
  
  // Reset MGTs
  setProgress(0.5,"Resetting MGTs");
  driver.channelMgr().resetLinks();
  
  // Reset AMC13 block
  setProgress(0.68,"Resetting AMC-AMC13 Link");
  driver.getReadout().resetAMC13Block();
  
  
  setProgress(0.83,"Resetting AMC-AMC13 Link");
  driver.resetPayload();

  setProgress(1.,"Reset completed");
   
  return kDone;
}

} //end ns mp7
} //end ns swatch
