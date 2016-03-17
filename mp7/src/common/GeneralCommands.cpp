
#include "swatch/mp7/GeneralCommands.hpp"


#include "xdata/String.h"
#include "xdata/Integer.h"
#include "xdata/UnsignedInteger.h"

#include "mp7/MP7Controller.hpp"
#include "mp7/PathConfigurator.hpp"
#include "mp7/TTCNode.hpp"

#include "swatch/mp7/MP7AbstractProcessor.hpp"


namespace swatch {
namespace mp7 {


ResetCommand::ResetCommand(const std::string& aId, swatch::core::ActionableObject& aActionable):
  Command(aId, aActionable, xdata::Integer()) 
{
  registerParameter("clockSource", xdata::String("external"));
  registerParameter("clockConfig", xdata::String("external"));
  registerParameter("ttcConfig", xdata::String("external"));
}
    
    
core::Command::State ResetCommand::code(const core::XParameterSet& params)
{
     
  // setProgress(0., "Resetting MP7");
  
  MP7AbstractProcessor& p = getActionable<MP7AbstractProcessor>();
  ::mp7::MP7Controller& driver = p.driver();
  setProgress(0.,"Resetting clocks");

  std::string mode = params.get<xdata::String>("clockSource").value_;
  std::string clkCfg = params.get<xdata::String>("clockConfig").value_;
  std::string ttcCfg = params.get<xdata::String>("ttcConfig").value_;

  if ( mode.compare("internal") && mode.compare("external") ) {
      setStatusMsg("Clock can be set only to 'internal' or 'external'");
      return State::kError;
  }

  try {
  // TODO: acquire lock
    // std::string mode = "external";
    driver.reset(mode, clkCfg, ttcCfg);
  } catch ( ::mp7::exception &e ) {
    
    setStatusMsg("Reset failed: "+e.description());
    return State::kError;
  }
  
  // Reset MGTs
  setProgress(0.5,"Resetting MGTs");
  driver.channelMgr().resetMGTs();
  
  // Reset AMC13 block
  setProgress(0.68,"Resetting AMC-AMC13 Link");
  driver.getReadout().resetAMC13Block();
  
  
  setProgress(0.83,"Resetting AMC-AMC13 Link");
  driver.resetPayload();

  setProgress(1.,"Reset completed");
   
  return State::kDone;
}

//---
SetIDCommand::SetIDCommand(const std::string& aId, swatch::core::ActionableObject& aActionable):
Command(aId, aActionable, xdata::UnsignedInteger())
{
  registerParameter("boardId", xdata::UnsignedInteger(0x0));
}

//---
core::Command::State 
SetIDCommand::code(const core::XParameterSet& params)
{
  MP7AbstractProcessor& p = getActionable<MP7AbstractProcessor>();
  ::mp7::MP7Controller& driver = p.driver();

  uint32_t boardId = params.get<xdata::UnsignedInteger>("boardId").value_;

  const ::mp7::CtrlNode& lCtrl = driver.getCtrl();

  // FIXME: replace with CtrlNode command when implemented
  lCtrl.getNode("board_id").write(boardId);
  lCtrl.getClient().dispatch();

  setResult(xdata::UnsignedInteger(boardId));
  return State::kDone;
}

} // namespace mp7
} // namespace swatch
