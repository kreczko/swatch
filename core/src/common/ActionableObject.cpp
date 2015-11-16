 
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


// STATIC MEMBERS INITIALIZATION
const std::string ActionableStatus::kNullStateMachineId = "NullStateMachine";
const std::string ActionableStatus::kNullStateId = "NoState";

//------------------------------------------------------------------------------------
bool ActionableStatus::isAlive() const {
  return mAlive;
}

//------------------------------------------------------------------------------------
bool ActionableStatus::isEngaged() const {
  return mStateMachineId != kNullStateMachineId;
}


//------------------------------------------------------------------------------------
bool ActionableStatus::isRunning() const {
  return !mRunningActions.empty();
}

//------------------------------------------------------------------------------------
const std::string& ActionableStatus::getState() const {
  return mState;
}

//------------------------------------------------------------------------------------
const Functionoid* ActionableStatus::getLastRunningAction() const {
  return mRunningActions.back();
}


//------------------------------------------------------------------------------------
const std::vector<const Functionoid*>& ActionableStatus::getRunningActions() const {
  return mRunningActions;
}


//------------------------------------------------------------------------------------
ActionableStatus::ActionableStatus() :
  mAlive(true), 
  mStateMachineId(kNullStateMachineId),
  mState(kNullStateId) {
}

const std::string& ActionableStatus::getStateMachineId() const {
  return mStateMachineId;
}


//------------------------------------------------------------------------------------
ObjectFunctionoid::ObjectFunctionoid(const std::string& aId, ActionableObject& aActionable) :
Functionoid(aId, aActionable) {
}


//------------------------------------------------------------------------------------
const ActionableObject& ObjectFunctionoid::getActionable() const {
	
	try {
		return getResource<const ActionableObject>();
	} catch ( std::bad_cast& lBadCast) {
		throw InvalidResource("Command '"+getPath()+"' failed to retrieve Actionable object");
	}
	
}


//------------------------------------------------------------------------------------
ActionableObject& ObjectFunctionoid::getActionable() {
	
	try {
		return getResource<ActionableObject>();
	} catch ( std::bad_cast& lBadCast) {
		throw InvalidResource("Command '"+getPath()+"' failed to retrieve Actionable object");
	}

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
void ActionableObject::engageStateMachine(const std::string& aFSM) {
  boost::lock_guard<boost::mutex> lGuard(mMutex);
  
  // Throw if currently in other state machine
  if(mStatus.getStateMachineId() != ActionableStatus::kNullStateMachineId )
    throw ResourceInWrongStateMachine("Cannot engage other state machine; resource '"+getPath()+"' currently in state machine '"+mStatus.getStateMachineId()+"'");

  const StateMachine& lOp = getStateMachine(aFSM);
//  mStatus.mFSM = & lOp;
  mStatus.mStateMachineId = lOp.getId();
  mStatus.mState = lOp.getInitialState();
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
ActionableObject::BusyGuard::BusyGuard(ActionableObject& aResource, const Functionoid& aFunctionoid, const BusyGuard* aOuterGuard) : 
  mResource(aResource),
  mAction(aFunctionoid),
  mOuterGuard(aOuterGuard)
{
  const boost::unique_lock<boost::mutex> lGuard(aResource.mMutex);
  initialise(lGuard);
}


//------------------------------------------------------------------------------------
ActionableObject::BusyGuard::BusyGuard(ActionableObject& aResource, const boost::unique_lock<boost::mutex>& aLockGuard, const Functionoid& aFunctionoid, const BusyGuard* aOuterGuard) : 
  mResource(aResource),
  mAction(aFunctionoid),
  mOuterGuard(aOuterGuard)
{
  initialise(aLockGuard);
}


//------------------------------------------------------------------------------------
void ActionableObject::BusyGuard::initialise(const boost::unique_lock<boost::mutex>& aLockGuard)
{
  if ( aLockGuard.mutex() != &mResource.mMutex)
    throw std::invalid_argument("BusyGuard's mutex lock guard is for wrong mutex");
  if ( ! aLockGuard.owns_lock() )
    throw std::invalid_argument("BusyGuard's mutex lock guard must own the lock");
  
  // Consistency checks on outer busy guard
  if(mOuterGuard != NULL)
  {
    if (&mOuterGuard->mResource != &mResource)
      throw WrongBusyGuard( "Incompatible outer BusyGuard, resource='"+mOuterGuard->mResource.getPath()+"'. Inner guard resource is '"+mResource.getPath() );
    else if ( mResource.mStatus.mRunningActions.empty() )
      throw WrongBusyGuard( "Outer BusyGuard used (resource: '"+mResource.getPath()+"', action: '"+mOuterGuard->mAction.getId()+"'), but resource not busy");
    else if ( &mOuterGuard->mAction != mResource.mStatus.getLastRunningAction() )
      throw WrongBusyGuard( "Outer BusyGuard (resource: '"+mResource.getPath()+"', action: '"+mOuterGuard->mAction.getId()+"') is not for current action '"+mResource.mStatus.getLastRunningAction()->getId()+"'" );
  }

  // 0) Check that this this action isn't already running
  if (std::count(mResource.mStatus.mRunningActions.begin(), mResource.mStatus.mRunningActions.end(), &mAction) > 0 )
    throw ActionableObjectIsBusy( "Action '"+mAction.getId()+"' is already running on resource '"+mResource.getPath()+"'" );

  // 1) For transitions, check that state machine is engaged, and we're in the right state
  if (const StateMachine::Transition* t = dynamic_cast<const StateMachine::Transition*>(&mAction))
  {
	if( mResource.mStatus.getStateMachineId() != t->getStateMachine().getId())
      throw ResourceInWrongState("Resource '"+mResource.getPath()+"' is not yet engaged in state machine '"+t->getStateMachine().getId()+"'");
    else if ( mResource.mStatus.mState != t->getStartState() )
      throw ResourceInWrongState("Resource '"+mResource.getPath()+"' is in state '"+mResource.mStatus.mState+"'; transition '"+t->getId()+"' cannot be run");
  }
  
  // 2) Claim the resource if free; else throw if can't get sole control of it
  if ( mResource.mStatus.isAlive() && ( (mOuterGuard != NULL) || !mResource.mStatus.isRunning() ) )
  {
    if ( !mResource.mStatus.isRunning() )
      LOG(swatch::logger::kInfo) << mResource.getPath() << " : Starting " << ActionFmt(&mAction);
    else
      LOG(swatch::logger::kInfo) << mResource.getPath() << " : Starting " << ActionFmt(&mAction) << " within " << ActionFmt(mResource.mStatus.getLastRunningAction());
    mResource.mStatus.mRunningActions.push_back(&mAction);
  }
  else
  {
    std::ostringstream oss;
    oss << "Could not run action '" << mAction.getId() << "' on resource '" << mResource.getPath() << "'. ";

    if ( mResource.mStatus.isRunning() )
	  oss << "Resource currently busy running functionoid '" << mResource.mStatus.getLastRunningAction()->getId() << "'.";
    else
      oss << "Actions currently disabled on this resource.";

   LOG(swatch::logger::kNotice) << oss.str();
   throw ActionableObjectIsBusy(oss.str());
  } 
}




ActionableObject::BusyGuard::~BusyGuard()
{  
  boost::lock_guard<boost::mutex> lGuard(mResource.mMutex);
  LOG(swatch::logger::kInfo) << mResource.getPath() << " : Finished " << ActionFmt(&mAction);
  if ( mResource.mStatus.isRunning() && (&mAction == mResource.mStatus.getLastRunningAction()) )
  {
    mResource.mStatus.mRunningActions.pop_back();
    
    // In case this was a transition, also update object's current state
    if (const StateMachine::Transition* t = dynamic_cast<const StateMachine::Transition*>(&mAction))
    {
      if(t->getStatus().getState() == ActionStatus::kError)
        mResource.mStatus.mState = t->getStateMachine().getErrorState();
      else
        mResource.mStatus.mState = t->getEndState();
    }
  }
  else
  {
    size_t lNrActions = mResource.mStatus.mRunningActions.size();
    const std::string activeFuncId(lNrActions > 0 ? "NULL" : "'" + mResource.mStatus.getLastRunningAction()->getId() + "' (innermost of "+boost::lexical_cast<std::string>(lNrActions)+")");
    LOG(swatch::logger::kError) << "unexpected active functionoid " << activeFuncId << "  in BusyGuard destructor for resource '" << mResource.getPath() << "', functionoid '" << mAction.getId() << "'";
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
