 
#include "swatch/core/ActionableObject.hpp"


#include "boost/foreach.hpp"

#include "swatch/core/Command.hpp"
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/Utilities.hpp"
#include "swatch/logger/Log.hpp"
#include "swatch/core/StateMachine.hpp"


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
ActionableObject::ActionableObject( const std::string& aId ) :
  MonitorableObject( aId  ),
  mStatus() {
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

  StateMachine* lFSM = new StateMachine(aId, *this, aInitialState, aErrorState);
  mFSMs.insert( std::make_pair( aId , lFSM ) );
  return *lFSM;
}


//------------------------------------------------------------------------------------
ActionableObject::Status ActionableObject::getStatus() const {
  boost::lock_guard<boost::mutex> lGuard(mMutex);
  return mStatus;
}


//------------------------------------------------------------------------------------
void ActionableObject::Deleter::operator ()(Object* aObject) {
  if(ActionableObject* lActionableObj = dynamic_cast<ActionableObject*>(aObject))
  {
    LOG(swatch::logger::kNotice) << aObject->getPath() << " : ActionableObject deleter called";

    lActionableObj->kill();

    //TODO (low-ish priority): Eventually replace this "spinning" do-loop with a more efficient implementation based on ActionableObject/Functionoid methods that use conditional variables behind-the-scenes 
    do {
    } while ( lActionableObj->getStatus().isRunning() );

    LOG(swatch::logger::kNotice) << aObject->getPath() << " : ActionableObject now being deleted";
    
    delete lActionableObj;
  }
  else{
    LOG(swatch::logger::kWarning) << "ActionableObject::Deleter being used on object '" << aObject->getPath() << "' of type '" << aObject->getTypeName() << "' that doesn't inherit from ActionableObject";
    delete aObject;
  }
}


//------------------------------------------------------------------------------------
void ActionableObject::kill(){
  boost::lock_guard<boost::mutex> lGuard(mMutex);
  mStatus.mAlive = false;
}



//------------------------------------------------------------------------------------
ActionableObject::BusyGuard::BusyGuard(ObjectFunctionoid& aAction, const BusyGuard* aOuterGuard) : 
  mActionableObj(aAction.getActionable()),
  mAction(aAction),
  mOuterGuard(aOuterGuard)
{
  const boost::unique_lock<boost::mutex> lGuard(mActionableObj.mMutex);
  initialise(lGuard);
}


//------------------------------------------------------------------------------------
ActionableObject::BusyGuard::BusyGuard(ActionableObject& aResource, const boost::unique_lock<boost::mutex>& aLockGuard, const Functionoid& aAction, const BusyGuard* aOuterGuard) : 
  mActionableObj(aResource),
  mAction(aAction),
  mOuterGuard(aOuterGuard)
{
  initialise(aLockGuard);
}


//------------------------------------------------------------------------------------
void ActionableObject::BusyGuard::initialise(const boost::unique_lock<boost::mutex>& aLockGuard)
{
  if ( aLockGuard.mutex() != &mActionableObj.mMutex)
    throw std::invalid_argument("BusyGuard's mutex lock guard is for wrong mutex");
  if ( ! aLockGuard.owns_lock() )
    throw std::invalid_argument("BusyGuard's mutex lock guard must own the lock");
  
  // Consistency checks on outer busy guard
  if(mOuterGuard != NULL)
  {
    if (&mOuterGuard->mActionableObj != &mActionableObj)
      throw WrongBusyGuard( "Incompatible outer BusyGuard, resource='"+mOuterGuard->mActionableObj.getPath()+"'. Inner guard resource is '"+mActionableObj.getPath() );
    else if ( mActionableObj.mStatus.mRunningActions.empty() )
      throw WrongBusyGuard( "Outer BusyGuard used (resource: '"+mActionableObj.getPath()+"', action: '"+mOuterGuard->mAction.getId()+"'), but resource not busy");
    else if ( &mOuterGuard->mAction != mActionableObj.mStatus.getLastRunningAction() )
      throw WrongBusyGuard( "Outer BusyGuard (resource: '"+mActionableObj.getPath()+"', action: '"+mOuterGuard->mAction.getId()+"') is not for current action '"+mActionableObj.mStatus.getLastRunningAction()->getId()+"'" );
  }

  // 0) Check that this this action isn't already running
  if (std::count(mActionableObj.mStatus.mRunningActions.begin(), mActionableObj.mStatus.mRunningActions.end(), &mAction) > 0 )
    throw ActionableObjectIsBusy( "Action '"+mAction.getId()+"' is already running on resource '"+mActionableObj.getPath()+"'" );

  // 1) For transitions, check that state machine is engaged, and we're in the right state
  if (const StateMachine::Transition* t = dynamic_cast<const StateMachine::Transition*>(&mAction))
  {
	if( mActionableObj.mStatus.getStateMachineId() != t->getStateMachine().getId())
      throw ResourceInWrongState("Resource '"+mActionableObj.getPath()+"' is not yet engaged in state machine '"+t->getStateMachine().getId()+"'");
    else if ( mActionableObj.mStatus.mState != t->getStartState() )
      throw ResourceInWrongState("Resource '"+mActionableObj.getPath()+"' is in state '"+mActionableObj.mStatus.mState+"'; transition '"+t->getId()+"' cannot be run");
  }
  
  // 2) Claim the resource if free; else throw if can't get sole control of it
  if ( mActionableObj.mStatus.isAlive() && ( (mOuterGuard != NULL) || !mActionableObj.mStatus.isRunning() ) )
  {
    if ( !mActionableObj.mStatus.isRunning() )
      LOG(swatch::logger::kInfo) << mActionableObj.getPath() << " : Starting " << ActionFmt(&mAction);
    else
      LOG(swatch::logger::kInfo) << mActionableObj.getPath() << " : Starting " << ActionFmt(&mAction) << " within " << ActionFmt(mActionableObj.mStatus.getLastRunningAction());
    mActionableObj.mStatus.mRunningActions.push_back(&mAction);
  }
  else
  {
    std::ostringstream oss;
    oss << "Could not run action '" << mAction.getId() << "' on resource '" << mActionableObj.getPath() << "'. ";

    if ( mActionableObj.mStatus.isRunning() )
	  oss << "Resource currently busy running functionoid '" << mActionableObj.mStatus.getLastRunningAction()->getId() << "'.";
    else
      oss << "Actions currently disabled on this resource.";

   LOG(swatch::logger::kNotice) << oss.str();
   throw ActionableObjectIsBusy(oss.str());
  } 
}




ActionableObject::BusyGuard::~BusyGuard()
{  
  boost::lock_guard<boost::mutex> lGuard(mActionableObj.mMutex);
  LOG(swatch::logger::kInfo) << mActionableObj.getPath() << " : Finished " << ActionFmt(&mAction);
  if ( mActionableObj.mStatus.isRunning() && (&mAction == mActionableObj.mStatus.getLastRunningAction()) )
  {
    mActionableObj.mStatus.mRunningActions.pop_back();
    
    // In case this was a transition, also update object's current state
    if (const StateMachine::Transition* t = dynamic_cast<const StateMachine::Transition*>(&mAction))
    {
      if(t->getStatus().getState() == ActionStatus::kError)
        mActionableObj.mStatus.mState = t->getStateMachine().getErrorState();
      else
        mActionableObj.mStatus.mState = t->getEndState();
    }
  }
  else
  {
    size_t lNrActions = mActionableObj.mStatus.mRunningActions.size();
    const std::string activeFuncId(lNrActions > 0 ? "NULL" : "'" + mActionableObj.mStatus.getLastRunningAction()->getId() + "' (innermost of "+boost::lexical_cast<std::string>(lNrActions)+")");
    LOG(swatch::logger::kError) << "unexpected active functionoid " << activeFuncId << "  in BusyGuard destructor for resource '" << mActionableObj.getPath() << "', functionoid '" << mAction.getId() << "'";
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

}
}
