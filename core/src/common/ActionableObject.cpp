 
#include "swatch/core/ActionableObject.hpp"


#include "boost/foreach.hpp"

#include "swatch/core/Command.hpp"
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/Utilities.hpp"
#include "swatch/logger/Logger.hpp"
#include "swatch/core/StateMachine.hpp"

//log4cplus headers
#include <log4cplus/loggingmacros.h>

using namespace std;

namespace swatch {
namespace core {

//------------------------------------------------------------------------------------
ObjectFunctionoid::ObjectFunctionoid(const std::string& aId, ActionableObject& aActionable) :
  Functionoid(aId),
  mActionable(aActionable)
{
}


//------------------------------------------------------------------------------------
const ActionableObject& ObjectFunctionoid::getActionable() const {
  return mActionable;
}


//------------------------------------------------------------------------------------
ActionableObject& ObjectFunctionoid::getActionable() {
  return mActionable;
}


//------------------------------------------------------------------------------------
ActionableObject::ActionableObject( const std::string& aId, const std::string& aLoggerName ) :
  MonitorableObject( aId  ),
  mStatus(),
  mLogger(swatch::logger::Logger::getInstance(aLoggerName))
{
  setMonitorableStatus(mStatus);
}


//------------------------------------------------------------------------------------
ActionableObject::~ActionableObject() {
}

//------------------------------------------------------------------------------------
CommandSequence& ActionableObject::getSequence( const std::string& aId )
{
  try {
    return *(mCommandSequences.at( aId ));
  } catch ( const std::out_of_range& e ) {
    throw CommandSequenceNotFoundInActionableObject( "Unable to find CommandSequence with ID '" + aId + "' in object '" + getPath() + "'" );
  }
}


//------------------------------------------------------------------------------------
Command& ActionableObject::getCommand( const std::string& aId )
{
  try {
    return *(mCommands.at( aId ));
  } catch ( const std::out_of_range& e ) {
    throw CommandNotFoundInActionableObject( "Unable to find Command with ID '" + aId + "' in object '" + getPath() + "'" );
  }
}


//------------------------------------------------------------------------------------
StateMachine& ActionableObject::getStateMachine( const std::string& aId )
{
  try {
    return *(mFSMs.at( aId ));
  } catch ( const std::out_of_range& e ) {
    throw StateMachineNotFoundInActionableObject( "Unable to find state machine with ID '" + aId + "' in object '" + getPath() + "'");
  }
}


//------------------------------------------------------------------------------------
std::set<std::string> ActionableObject::getSequences() const
{
  std::set<std::string> lNames;
  BOOST_FOREACH( CommandSequenceMap_t::value_type p, mCommandSequences) {
    lNames.insert( p.first );
  }
  return lNames;
}

std::set<std::string> ActionableObject::getCommands() const
{
  std::set<std::string> lNames;
  BOOST_FOREACH( CommandMap_t::value_type p, mCommands) {
    lNames.insert( p.first );
  }
  return lNames;
}

std::set<std::string> ActionableObject::getStateMachines() const
{
  std::set<std::string> lNames;
  BOOST_FOREACH( StateMachineMap_t::value_type p, mFSMs) {
    lNames.insert( p.first );
  }
  return lNames;
}

//------------------------------------------------------------------------------------
CommandSequence& ActionableObject::registerSequence( const std::string& aId, const std::string& aFirstCommandId, const std::string& aFirstCommandNamespace)
{ 
  if (mCommandSequences.count(aId)) {
    throw CommandSequenceAlreadyExistsInActionableObject( "CommandSequence With ID '"+aId+"' already exists" );
  }
  CommandSequence* lSequence = new CommandSequence(aId, *this, mStatus, aFirstCommandId, aFirstCommandNamespace);
  addObj(lSequence);
  mCommandSequences.insert( std::make_pair( aId , lSequence ) );
  return *lSequence;
}


//------------------------------------------------------------------------------------
CommandSequence& ActionableObject::registerSequence( const std::string& aId, Command& aFirstCommand, const std::string& aFirstCommandNamespace)
{ 
  if (mCommandSequences.count(aId)) {
    throw CommandSequenceAlreadyExistsInActionableObject( "CommandSequence With ID '"+aId+"' already exists" );
  }
  CommandSequence* lSequence = new CommandSequence(aId, *this, mStatus, aFirstCommand, aFirstCommandNamespace);
  addObj(lSequence);
  mCommandSequences.insert( std::make_pair( aId , lSequence ) );
  return *lSequence;
}


//------------------------------------------------------------------------------------
Command& ActionableObject::registerCommand( const std::string& aId , Command* aCommand )
{
  if (mCommands.count(aId)){
    delete aCommand;
    throw CommandAlreadyExistsInActionableObject( "Command With ID '"+aId+"' already exists" );
  }
  this->addObj(aCommand);
  mCommands.insert( std::make_pair( aId , aCommand ) );
  return *aCommand;
}


//------------------------------------------------------------------------------------
StateMachine& ActionableObject::registerStateMachine( const std::string& aId, const std::string& aInitialState, const std::string& aErrorState ) {
  if (mFSMs.count(aId))
    throw StateMachineAlreadyExistsInActionableObject( "State machine With ID '"+aId+"' already exists" );

  StateMachine* lFSM = new StateMachine(aId, *this, mStatus, aInitialState, aErrorState);
  addObj(lFSM);
  mFSMs.insert( std::make_pair( aId , lFSM ) );
  return *lFSM;
}


//------------------------------------------------------------------------------------
ActionableObject::Status_t ActionableObject::getStatus() const
{
  ActionableStatusGuard lGuard(mStatus);
  return mStatus.getSnapshot(lGuard);
}


//------------------------------------------------------------------------------------
void ActionableObject::Deleter::operator ()(Object* aObject)
{
  if(ActionableObject* lActionableObj = dynamic_cast<ActionableObject*>(aObject))
  {
    LOG4CPLUS_INFO(lActionableObj->getLogger(), aObject->getPath() << " : ActionableObject deleter called");

    {
      ActionableStatusGuard lGuard(lActionableObj->mStatus);
      lActionableObj->mStatus.kill(lGuard);
    }

    //TODO (low-ish priority): Eventually replace this "spinning" do-loop with a more efficient implementation based on ActionableObject/Functionoid methods that use conditional variables behind-the-scenes 
    do {
    } while ( lActionableObj->getStatus().isRunning() );

    LOG4CPLUS_INFO(lActionableObj->getLogger(), aObject->getPath() << " : ActionableObject now being deleted");
    
    delete lActionableObj;
  }
  else{
    log4cplus::Logger lLogger = swatch::logger::Logger::getInstance("swatch.core.ActionableObject");
    LOG4CPLUS_WARN(lLogger, "ActionableObject::Deleter being used on object '" << aObject->getPath() << "' of type '" << aObject->getTypeName() << "' that doesn't inherit from ActionableObject");
    delete aObject;
  }
}


ActionableObject::ActionFmt::ActionFmt(const Functionoid* aAction) : 
  mAction(aAction)
{
}


ActionableObject::ActionFmt::~ActionFmt()
{
}


std::string ActionableObject::ActionFmt::str() const
{
  return boost::lexical_cast<std::string>(*this);
}

std::ostream& operator<<(std::ostream& aStream, const ActionableObject::ActionFmt& aActionFmt)
{
  if (dynamic_cast<const Command*>(aActionFmt.mAction) != NULL)
    aStream << "command";
  else if (dynamic_cast<const CommandSequence*>(aActionFmt.mAction) != NULL)
    aStream << "sequence";
  else if (dynamic_cast<const StateMachine::Transition*>(aActionFmt.mAction) != NULL)
    aStream << "transition";
  else
    aStream << "action";

  if( aActionFmt.mAction == NULL )
    aStream << " NULL";
  else
    aStream << " '" << aActionFmt.mAction->getId() << "'";

  return aStream;
}


//------------------------------------------------------------------------------------
BusyGuard::BusyGuard(ObjectFunctionoid& aAction, ActionableStatus& aStatus, const BusyGuard* aOuterGuard) : 
  mActionableObj(aAction.getActionable()),
  mStatus(aStatus),
  mAction(aAction),
  mOuterGuard(aOuterGuard)
{
  ActionableStatusGuard lGuard(mStatus);
  initialise(lGuard);
}


//------------------------------------------------------------------------------------
BusyGuard::BusyGuard(ObjectFunctionoid& aAction, ActionableStatus& aStatus, ActionableStatusGuard& aStatusGuard, const Callback_t& aCallback, const BusyGuard* aOuterGuard) : 
  mActionableObj(aAction.getActionable()),
  mStatus(aStatus),
  mAction(aAction),
  mOuterGuard(aOuterGuard),
  mPostActionCallback(aCallback)
{
  initialise(aStatusGuard);
}


//------------------------------------------------------------------------------------
BusyGuard::BusyGuard(ActionableObject& aResource, ActionableStatus& aStatus, ActionableStatusGuard& aStatusGuard, const Functionoid& aAction, const BusyGuard* aOuterGuard) : 
  mActionableObj(aResource),
  mStatus(aStatus),
  mAction(aAction),
  mOuterGuard(aOuterGuard)
{
  initialise(aStatusGuard);
}


//------------------------------------------------------------------------------------
BusyGuard::BusyGuard(ActionableObject& aResource, ActionableStatus& aStatus, ActionableStatusGuard& aStatusGuard, const Functionoid& aAction, const Adopt) : 
  mActionableObj(aResource),
  mStatus(aStatus),
  mAction(aAction),
  mOuterGuard(NULL)
{
  if ( mStatus.getSnapshot(aStatusGuard).isActionWaitingToRun() )
    throw WrongBusyGuard("BusyGuard cannot adopt action '"+mAction.getPath()+"' on object '"+mActionableObj.getPath()+"' : Object is currently waiting for an action to run");
  else if ( mStatus.getRunningActions(aStatusGuard).size() != size_t(1) )
    throw WrongBusyGuard("BusyGuard cannot adopt action '"+mAction.getPath()+"' on object '"+mActionableObj.getPath()+"' : Action stack contains "+boost::lexical_cast<std::string>(mStatus.getRunningActions(aStatusGuard).size())+" actions");
  else if ( mStatus.getRunningActions(aStatusGuard).at(0) != &mAction )
    throw WrongBusyGuard("BusyGuard cannot adopt action '"+mAction.getPath()+"' on object '"+mActionableObj.getPath()+"' : Object is running "+ActionFmt_t(mStatus.getRunningActions(aStatusGuard).at(0)).str());

  LOG4CPLUS_INFO(mActionableObj.getLogger(), mActionableObj.getPath() << " : Starting " << ActionFmt_t(&mAction) << " (adopted)");
}


//------------------------------------------------------------------------------------
void BusyGuard::initialise(ActionableStatusGuard& aStatusGuard)
{
  ActionableSnapshot lStatusSnapshot = mStatus.getSnapshot(aStatusGuard);
  // Consistency checks on outer busy guard
  if(mOuterGuard != NULL)
  {
    if (&mOuterGuard->mActionableObj != &mActionableObj)
      throw WrongBusyGuard( "Incompatible outer BusyGuard, resource='"+mOuterGuard->mActionableObj.getPath()+"'. Inner guard resource is '"+mActionableObj.getPath() );
    else if ( lStatusSnapshot.getRunningActions().empty() )
      throw WrongBusyGuard( "Outer BusyGuard used (resource: '"+mActionableObj.getPath()+"', action: '"+mOuterGuard->mAction.getId()+"'), but resource not busy");
    else if ( &mOuterGuard->mAction != lStatusSnapshot.getLastRunningAction() )
      throw WrongBusyGuard( "Outer BusyGuard (resource: '"+mActionableObj.getPath()+"', action: '"+mOuterGuard->mAction.getId()+"') is not for current action '"+lStatusSnapshot.getLastRunningAction()->getId()+"'" );
  }

  // 1) Check that this this action isn't already running
  if (std::count(lStatusSnapshot.getRunningActions().begin(), lStatusSnapshot.getRunningActions().end(), &mAction) > 0 )
    throw ActionableObjectIsBusy( "Action '"+mAction.getId()+"' is already running on resource '"+mActionableObj.getPath()+"'" );

  // 2) Claim the resource if free; else throw if can't get sole control of it
  if ( lStatusSnapshot.isAlive() && ( (mOuterGuard != NULL) || !lStatusSnapshot.isRunning() ) )
  {
    if ( !lStatusSnapshot.isRunning() )
    {
      LOG4CPLUS_INFO(mActionableObj.getLogger(), mActionableObj.getPath() << " : Starting " << ActionFmt_t(&mAction));
      mStatus.waitUntilReadyToRunAction(mAction, aStatusGuard);
    }
    else
    {
      LOG4CPLUS_INFO(mActionableObj.getLogger(), mActionableObj.getPath() << " : Starting " << ActionFmt_t(&mAction) << " within " << ActionFmt_t(lStatusSnapshot.getLastRunningAction()));
      mStatus.addAction(mAction, aStatusGuard);
    }
  }
  else
  {
    std::ostringstream oss;
    oss << "Could not run action '" << mAction.getId() << "' on resource '" << mActionableObj.getPath() << "'. ";

    if ( lStatusSnapshot.isRunning() )
	  oss << "Resource currently busy running functionoid '" << lStatusSnapshot.getLastRunningAction()->getId() << "'.";
    else
      oss << "Actions currently disabled on this resource.";

   LOG4CPLUS_INFO(mActionableObj.getLogger(), oss.str());
   throw ActionableObjectIsBusy(oss.str());
  } 
}

//------------------------------------------------------------------------------------
BusyGuard::~BusyGuard()
{  
  ActionableStatusGuard lGuard(mStatus);
  ActionableSnapshot lStatusSnapshot = mStatus.getSnapshot(lGuard);
  LOG4CPLUS_INFO(mActionableObj.getLogger(), mActionableObj.getPath() << " : Finished " << ActionFmt_t(&mAction));

  if ( lStatusSnapshot.isRunning() && (&mAction == lStatusSnapshot.getLastRunningAction()) )
  {
    mStatus.popAction(lGuard);
    
    if( ! mPostActionCallback.empty() )
      mPostActionCallback(lGuard);
  }
  else
  {
    size_t lNrActions = lStatusSnapshot.getRunningActions().size();
    const std::string activeFuncId(lNrActions > 0 ? "NULL" : "'" + lStatusSnapshot.getLastRunningAction()->getId() + "' (innermost of "+boost::lexical_cast<std::string>(lNrActions)+")");
    LOG4CPLUS_ERROR(mActionableObj.getLogger(), "unexpected active functionoid " << activeFuncId << "  in BusyGuard destructor for resource '" << mActionableObj.getPath() << "', functionoid '" << mAction.getId() << "'");
  }
}



log4cplus::Logger& ActionableObject::getLogger() {
  return mLogger;
}

}
}
