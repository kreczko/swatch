
#include "swatch/dummy/DummyProcessorCommands.hpp"


#include "swatch/dummy/DummyProcessor.hpp"
#include "swatch/dummy/DummyProcDriver.hpp"


namespace swatch {
namespace dummy {


//////////////////////////
/*  DummyRebootCommand  */

DummyRebootCommand::DummyRebootCommand(const std::string& aId, core::ActionableObject& aActionable) : 
  AbstractConfigureCommand(aId, aActionable)
{
}

DummyRebootCommand::~DummyRebootCommand()
{
}

void DummyRebootCommand::runAction(bool aGoIntoError)
{
  DummyProcDriver& lDriver = getActionable<DummyProcessor>().getDriver();
  lDriver.reboot();
}


/////////////////////////
/*  DummyResetCommand  */

DummyResetCommand::DummyResetCommand(const std::string& aId, core::ActionableObject& aActionable) : 
  AbstractConfigureCommand(aId, aActionable)
{
}

DummyResetCommand::~DummyResetCommand()
{
}

void DummyResetCommand::runAction(bool aGoIntoError)
{
  DummyProcDriver& lDriver = getActionable<DummyProcessor>().getDriver();
  if (!aGoIntoError)
    lDriver.reset();
}


///////////////////////////////
/*  DummyConfigureTxCommand  */

DummyConfigureTxCommand::DummyConfigureTxCommand(const std::string& aId, core::ActionableObject& aActionable) : 
  AbstractConfigureCommand(aId, aActionable)
{
}

DummyConfigureTxCommand::~DummyConfigureTxCommand()
{
}

void DummyConfigureTxCommand::runAction(bool aGoIntoError)
{
  DummyProcDriver& lDriver = getActionable<DummyProcessor>().getDriver();
  if (!aGoIntoError)
    lDriver.configureTxPorts();
}


///////////////////////////////
/*  DummyConfigureRxCommand  */

DummyConfigureRxCommand::DummyConfigureRxCommand(const std::string& aId, core::ActionableObject& aActionable) : 
  AbstractConfigureCommand(aId, aActionable)
{
}

DummyConfigureRxCommand::~DummyConfigureRxCommand()
{
}

void DummyConfigureRxCommand::runAction(bool aGoIntoError)
{
  DummyProcDriver& lDriver = getActionable<DummyProcessor>().getDriver();
  if (!aGoIntoError)
    lDriver.configureRxPorts();
}


////////////////////////////////
/*  DummyConfigureDaqCommand  */

DummyConfigureDaqCommand::DummyConfigureDaqCommand(const std::string& aId, core::ActionableObject& aActionable) : 
  AbstractConfigureCommand(aId, aActionable)
{
}

DummyConfigureDaqCommand::~DummyConfigureDaqCommand()
{
}

void DummyConfigureDaqCommand::runAction(bool aGoIntoError)
{
  DummyProcDriver& lDriver = getActionable<DummyProcessor>().getDriver();
  if (!aGoIntoError)
    lDriver.configureReadout();
}


/////////////////////////////////
/*  DummyConfigureAlgoCommand  */

DummyConfigureAlgoCommand::DummyConfigureAlgoCommand(const std::string& aId, core::ActionableObject& aActionable) : 
  AbstractConfigureCommand(aId, aActionable)
{
}

DummyConfigureAlgoCommand::~DummyConfigureAlgoCommand()
{
}

void DummyConfigureAlgoCommand::runAction(bool aGoIntoError)
{
  DummyProcDriver& lDriver = getActionable<DummyProcessor>().getDriver();
  if (!aGoIntoError)
    lDriver.configureAlgo();
}


/////////////////////////////////////////////
/*  DummyProcessorForceClkTtcStateCommand  */

DummyProcessorForceClkTtcStateCommand::DummyProcessorForceClkTtcStateCommand(const std::string& aId, core::ActionableObject& aActionable) :
  AbstractForceStateCommand(aId, aActionable)
{
}

DummyProcessorForceClkTtcStateCommand::~DummyProcessorForceClkTtcStateCommand()
{
}

core::Command::State DummyProcessorForceClkTtcStateCommand::code(const core::XParameterSet& aParamSet)
{
  DummyProcDriver& lDriver = getActionable<DummyProcessor>().getDriver();
  lDriver.forceClkTtcState(parseState(aParamSet));
  return core::ActionSnapshot::kDone;
}


//////////////////////////////////////////////
/*  DummyProcessorForceRxPortsStateCommand  */

DummyProcessorForceRxPortsStateCommand::DummyProcessorForceRxPortsStateCommand(const std::string& aId, core::ActionableObject& aActionable) :
  AbstractForceStateCommand(aId, aActionable)
{
}

DummyProcessorForceRxPortsStateCommand::~DummyProcessorForceRxPortsStateCommand()
{
}

core::Command::State DummyProcessorForceRxPortsStateCommand::code(const core::XParameterSet& aParamSet)
{
  DummyProcDriver& lDriver = getActionable<DummyProcessor>().getDriver();
  lDriver.forceRxPortsState(parseState(aParamSet));
  return core::ActionSnapshot::kDone;
}


//////////////////////////////////////////////
/*  DummyProcessorForceTxPortsStateCommand  */

DummyProcessorForceTxPortsStateCommand::DummyProcessorForceTxPortsStateCommand(const std::string& aId, core::ActionableObject& aActionable) :
  AbstractForceStateCommand(aId, aActionable)
{
}

DummyProcessorForceTxPortsStateCommand::~DummyProcessorForceTxPortsStateCommand()
{
}

core::Command::State DummyProcessorForceTxPortsStateCommand::code(const core::XParameterSet& aParamSet)
{
  DummyProcDriver& lDriver = getActionable<DummyProcessor>().getDriver();
  lDriver.forceTxPortsState(parseState(aParamSet));
  return core::ActionSnapshot::kDone;
}


//////////////////////////////////////////////
/*  DummyProcessorForceReadoutStateCommand  */

DummyProcessorForceReadoutStateCommand::DummyProcessorForceReadoutStateCommand(const std::string& aId, core::ActionableObject& aActionable) :
  AbstractForceStateCommand(aId, aActionable)
{
}

DummyProcessorForceReadoutStateCommand::~DummyProcessorForceReadoutStateCommand()
{
}

core::Command::State DummyProcessorForceReadoutStateCommand::code(const core::XParameterSet& aParamSet)
{
  DummyProcDriver& lDriver = getActionable<DummyProcessor>().getDriver();
  lDriver.forceReadoutState(parseState(aParamSet));
  return core::ActionSnapshot::kDone;
}


///////////////////////////////////////////
/*  DummyProcessorForceAlgoStateCommand  */

DummyProcessorForceAlgoStateCommand::DummyProcessorForceAlgoStateCommand(const std::string& aId, core::ActionableObject& aActionable) :
  AbstractForceStateCommand(aId, aActionable)
{
}

DummyProcessorForceAlgoStateCommand::~DummyProcessorForceAlgoStateCommand()
{
}

core::Command::State DummyProcessorForceAlgoStateCommand::code(const core::XParameterSet& aParamSet)
{
  DummyProcDriver& lDriver = getActionable<DummyProcessor>().getDriver();
  lDriver.forceAlgoState(parseState(aParamSet));
  return core::ActionSnapshot::kDone;
}



} // namespace dummy
} // namespace swatch
