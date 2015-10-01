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

  aResource.addObj(this);
}


CommandSequence::CommandSequence( const std::string& aId, ActionableObject& aResource, Command& aFirstCommand, const std::string& aFirstCommandAlias) :
  CommandVec(aId, aResource)
{
  run(aFirstCommand, aFirstCommandAlias);

  aResource.addObj(this);
}


CommandSequence::~CommandSequence() {
}


CommandSequence& CommandSequence::run( Command& aCommand, const std::string& aAlias )
{
  addCommand(aCommand, getId(), aAlias);
  return *this;
}


CommandSequence& CommandSequence::then ( Command& aCommand, const std::string& aAlias )
{
  return run( aCommand, aAlias );
}


CommandSequence& CommandSequence::run( const std::string& aCommand, const std::string& aAlias )
{
  addCommand(aCommand, getId(), aAlias);
  return *this;
}


CommandSequence& CommandSequence::then ( const std::string& aCommand, const std::string& aAlias )
{
  return run( aCommand, aAlias );
}


} /* namespace core */
} /* namespace swatch */
