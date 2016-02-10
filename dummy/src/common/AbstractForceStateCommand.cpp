
#include "swatch/dummy/AbstractForceStateCommand.hpp"


#include <algorithm>


#include "xdata/String.h"

#include "swatch/dummy/ComponentState.hpp"


namespace swatch {
namespace dummy {


AbstractForceStateCommand::AbstractForceStateCommand(const std::string& aId, core::ActionableObject& aActionable) : 
  Command(aId, aActionable, xdata::String("Dummy 'force state' command's default result!"))
{
  registerParameter("state", xdata::String("error"));
}


AbstractForceStateCommand::~AbstractForceStateCommand()
{
}


ComponentState AbstractForceStateCommand::parseState(const core::XParameterSet& aParams)
{
  std::string aStateParam = aParams.get<xdata::String>("state").value_;

  // Transform to lowercase
  std::transform(aStateParam.begin(), aStateParam.end(), aStateParam.begin(), ::tolower);

  if (aStateParam == "unreachable" || aStateParam == "not-reachable")
    return ComponentState::kNotReachable;
  else if (aStateParam == "error")
    return ComponentState::kError;
  else if (aStateParam == "warning")
    return ComponentState::kWarning;
  else if (aStateParam == "good")
    return ComponentState::kGood;
  else
    throw std::runtime_error("Invalid state string '" + aParams.get<xdata::String>("state").value_ + "'");
}

} // end ns: dummy
} // end ns: swatch
