
#include "swatch/dummy/AbstractConfigureCommand.hpp"


#include "xdata/Boolean.h"
#include "xdata/String.h"
#include "xdata/UnsignedInteger.h"


namespace swatch {
namespace dummy {


AbstractConfigureCommand::AbstractConfigureCommand(const std::string& aId, core::ActionableObject& aActionable) : 
  Command(aId, aActionable, xdata::String("Dummy command's default result!"))
{
  registerParameter("cmdDuration", xdata::UnsignedInteger(5));
  registerParameter("returnWarning", xdata::Boolean(false));
  registerParameter("returnError", xdata::Boolean(false));
  registerParameter("throw", xdata::Boolean(false));
}


AbstractConfigureCommand::~AbstractConfigureCommand()
{
}


core::Command::State AbstractConfigureCommand::code(const core::XParameterSet& aParams)
{  
  const size_t nrSeconds = aParams.get<xdata::UnsignedInteger>("cmdDuration").value_;

  core::Command::State lState = core::ActionSnapshot::kDone;
  if(aParams.get<xdata::Boolean>("throw").value_)
    lState = core::ActionSnapshot::kError;
  else if (aParams.get<xdata::Boolean>("returnError").value_)
    lState = core::ActionSnapshot::kError;
  else if (aParams.get<xdata::Boolean>("returnWarning").value_)
    lState = core::ActionSnapshot::kWarning;
  
  for(size_t i=0; i<(nrSeconds*4); i++)
  {
    std::ostringstream oss;
    oss << "Done " << i << " of " << (nrSeconds*4) << " things";
    setProgress(float(i)/float(nrSeconds*4), oss.str());
    boost::this_thread::sleep_for(boost::chrono::milliseconds(250));
  }

  this->runAction(lState == core::ActionSnapshot::kError);
  
  if (aParams.get<xdata::Boolean>("throw").value_)
    throw std::runtime_error("An exceptional error occurred!");
  return lState;
}

} // end ns: dummy
} // end ns: swatch
