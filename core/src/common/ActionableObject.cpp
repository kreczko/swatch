 
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
  mLogger(swatch::logger::Logger::getInstance(aLoggerName)) {
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
  BOOST_FOREACH( tCommandSequenceMap::value_type p, mCommandSequences) {
    lNames.insert( p.first );
  }
  return lNames;
}

std::set<std::string> ActionableObject::getCommands() const
{
  std::set<std::string> lNames;
  BOOST_FOREACH( tCommandMap::value_type p, mCommands) {
    lNames.insert( p.first );
  }
  return lNames;
}

std::set<std::string> ActionableObject::getStateMachines() const
{
  std::set<std::string> lNames;
  BOOST_FOREACH( tStateMachineMap::value_type p, mFSMs) {
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
  CommandSequence* lSequence = new CommandSequence(aId, *this, aFirstCommandId, aFirstCommandNamespace);
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
  CommandSequence* lSequence = new CommandSequence(aId, *this, aFirstCommand, aFirstCommandNamespace);
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
ActionableObject::Status ActionableObject::getStatus() const
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


//------------------------------------------------------------------------------------
ActionableObject::BusyGuard::BusyGuard(ObjectFunctionoid& aAction, const BusyGuard* aOuterGuard) : 
  mActionableObj(aAction.getActionable()),
  mStatus(aAction.getActionable().mStatus),
  mAction(aAction),
  mOuterGuard(aOuterGuard)
{
  ActionableStatusGuard lGuard(mStatus);
  initialise(lGuard);
}


//------------------------------------------------------------------------------------
ActionableObject::BusyGuard::BusyGuard(ActionableObject& aResource, const ActionableStatusGuard& aStatusGuard, const Functionoid& aAction, const BusyGuard* aOuterGuard) : 
  mActionableObj(aResource),
  mStatus(aResource.mStatus),
  mAction(aAction),
  mOuterGuard(aOuterGuard)
{
  initialise(aStatusGuard);
}


//------------------------------------------------------------------------------------
void ActionableObject::BusyGuard::initialise(const ActionableStatusGuard& aStatusGuard)
{
  Status lStatusSnapshot = mStatus.getSnapshot(aStatusGuard);
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

  // 0) Check that this this action isn't already running
  if (std::count(lStatusSnapshot.getRunningActions().begin(), lStatusSnapshot.getRunningActions().end(), &mAction) > 0 )
    throw ActionableObjectIsBusy( "Action '"+mAction.getId()+"' is already running on resource '"+mActionableObj.getPath()+"'" );

  // 1) For transitions, check that state machine is engaged, and we're in the right state
  if (const StateMachine::Transition* t = dynamic_cast<const StateMachine::Transition*>(&mAction))
  {
	if( lStatusSnapshot.getStateMachineId() != t->getStateMachine().getId())
      throw ResourceInWrongState("Resource '"+mActionableObj.getPath()+"' is not yet engaged in state machine '"+t->getStateMachine().getId()+"'");
        else if ( lStatusSnapshot.getState() != t->getStartState() )
      throw ResourceInWrongState("Resource '"+mActionableObj.getPath()+"' is in state '"+lStatusSnapshot.getState()+"'; transition '"+t->getId()+"' cannot be run");
  }
  
  // 2) Claim the resource if free; else throw if can't get sole control of it
  if ( lStatusSnapshot.isAlive() && ( (mOuterGuard != NULL) || !lStatusSnapshot.isRunning() ) )
  {
    if ( !lStatusSnapshot.isRunning() )
      LOG4CPLUS_INFO(mActionableObj.getLogger(), mActionableObj.getPath() << " : Starting " << ActionFmt(&mAction));
    else
      LOG4CPLUS_INFO(mActionableObj.getLogger(), mActionableObj.getPath() << " : Starting " << ActionFmt(&mAction) << " within " << ActionFmt(lStatusSnapshot.getLastRunningAction()));
    mStatus.addAction(mAction, aStatusGuard);
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




ActionableObject::BusyGuard::~BusyGuard()
{  
  ActionableStatusGuard lGuard(mStatus);
  Status lStatusSnapshot = mStatus.getSnapshot(lGuard);
  LOG4CPLUS_INFO(mActionableObj.getLogger(), mActionableObj.getPath() << " : Finished " << ActionFmt(&mAction));

  if ( lStatusSnapshot.isRunning() && (&mAction == lStatusSnapshot.getLastRunningAction()) )
  {
    mStatus.popAction(lGuard);
    
    // In case this was a transition, also update object's current state
    if (const StateMachine::Transition* t = dynamic_cast<const StateMachine::Transition*>(&mAction))
    {
      if(t->getStatus().getState() == ActionStatus::kError)
        mStatus.setState(t->getStateMachine().getErrorState(), lGuard);
      else
        mStatus.setState(t->getEndState(), lGuard);
    }
  }
  else
  {
    size_t lNrActions = lStatusSnapshot.getRunningActions().size();
    const std::string activeFuncId(lNrActions > 0 ? "NULL" : "'" + lStatusSnapshot.getLastRunningAction()->getId() + "' (innermost of "+boost::lexical_cast<std::string>(lNrActions)+")");
    LOG4CPLUS_ERROR(mActionableObj.getLogger(), "unexpected active functionoid " << activeFuncId << "  in BusyGuard destructor for resource '" << mActionableObj.getPath() << "', functionoid '" << mAction.getId() << "'");
  }
}


ActionableObject::ActionFmt::ActionFmt(const Functionoid* aAction) : 
  mAction(aAction)
{
}


ActionableObject::ActionFmt::~ActionFmt()
{
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

log4cplus::Logger& ActionableObject::getLogger() {
  return mLogger;
}

}
}
