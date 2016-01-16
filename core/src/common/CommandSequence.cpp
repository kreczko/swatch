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


CommandSequence::CommandSequence( const std::string& aId, ActionableObject& aResource, ActionableStatus& aActionableStatus, const std::string& aFirstCommandId, const std::string& aFirstCommandAlias) :
  CommandVec(aId, aResource),
  mActionableStatus(aActionableStatus)
{
  run(aFirstCommandId, aFirstCommandAlias);
}


CommandSequence::CommandSequence( const std::string& aId, ActionableObject& aResource, ActionableStatus& aActionableStatus, Command& aFirstCommand, const std::string& aFirstCommandAlias) :
  CommandVec(aId, aResource),
  mActionableStatus(aActionableStatus)
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


//------------------------------------------------------------------------------------
void CommandSequence::exec(const GateKeeper& aGateKeeper, const bool& aUseThreadPool )
{
  exec(NULL, aGateKeeper, aUseThreadPool);
}


//------------------------------------------------------------------------------------
void CommandSequence::exec(const BusyGuard* aOuterBusyGuard, const GateKeeper& aGateKeeper, const bool& aUseThreadPool )
{
  // 1) Extract parameters before creating busy guard (so that resource doesn't change states if parameter is missing)
  std::vector<ReadOnlyXParameterSet> lParamSets;
  std::vector<MissingParam> lMissingParams;
  extractParameters(aGateKeeper, lParamSets, lMissingParams, true);

  // 2) Create busy guard
  boost::shared_ptr<BusyGuard> lBusyGuard(new BusyGuard(*this, mActionableStatus, aOuterBusyGuard));

// FIXME: Re-implement parameter cache at some future date; disabled by Tom on 28th August, since ...
//        ... current logic doesn't work correctly with different gatekeepers - need to change to ...
//        ... updating cache if either gatekeeper filename/runkey different or cache update timestamp older than gatekeeper.lastUpdated 
//  // Is our cache of parameters up to date?
//  boost::posix_time::ptime lUpdateTime( aGateKeeper.lastUpdated() );
//  if( mParamUpdateTime != lUpdateTime )
//  {
//    updateParameterCache(aGateKeeper);
//    mParamUpdateTime = lUpdateTime; // We are up to date :)
//  }
  
  // 3) Reset this sequence's state variables
  reset(lParamSets);

  // 4) Run the commands
  if (aUseThreadPool) {
    scheduleAction<CommandSequence>(this, &CommandSequence::runCommands, lBusyGuard);
  }
  else {
    // otherwise execute in same thread
    this->runCommands(lBusyGuard);
  }
}


} /* namespace core */
} /* namespace swatch */
