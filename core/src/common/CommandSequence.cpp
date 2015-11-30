#include "swatch/core/CommandSequence.hpp"


#include <iostream>
#include <vector>

#include "swatch/core/ActionableObject.hpp"
#include "swatch/core/Command.hpp"
#include "swatch/core/GateKeeper.hpp"
#include "swatch/core/XParameterSet.hpp"
#include "swatch/core/ThreadPool.hpp"
#include "swatch/logger/Log.hpp"


namespace swatch {
namespace core {


CommandSequence::CommandSequence( const std::string& aId, ActionableObject& aResource, const std::string& aFirstCommandId, const std::string& aFirstCommandAlias) :
  CommandVec(aId, aResource)
{
  run(aFirstCommandId, aFirstCommandAlias);
}


CommandSequence::CommandSequence( const std::string& aId, ActionableObject& aResource, Command& aFirstCommand, const std::string& aFirstCommandAlias) :
  CommandVec(aId, aResource)
{
  run(aFirstCommand, aFirstCommandAlias);
}


CommandSequence::~CommandSequence() {
}


CommandSequence& CommandSequence::run( Command& aCommand, const std::string& aNamespace )
{
  addCommand(aCommand, (aNamespace.empty() ? getId() : aNamespace) );
  return *this;
}


CommandSequence& CommandSequence::then ( Command& aCommand, const std::string& aNamespace )
{
  return run( aCommand, (aNamespace.empty() ? getId() : aNamespace) );
}


CommandSequence& CommandSequence::run( const std::string& aCommand, const std::string& aNamespace )
{
  addCommand(aCommand, (aNamespace.empty() ? getId() : aNamespace));
  return *this;
}


CommandSequence& CommandSequence::then ( const std::string& aCommand, const std::string& aNamespace )
{
  return run( aCommand, (aNamespace.empty() ? getId() : aNamespace) );
}

void CommandSequence::prepareCommands(const tReadOnlyXParameterSets& aParameters, const tMonitoringSettings& aMonSettings) {

}
void CommandSequence::finaliseCommands(const tReadOnlyXParameterSets& aParameters, const tMonitoringSettings& aMonSettings) {

}

void CommandSequence::extractMonitoringSettings(const GateKeeper& aGateKeeper, tMonitoringSettings& aMonSettings) const {

}


} /* namespace core */
} /* namespace swatch */
