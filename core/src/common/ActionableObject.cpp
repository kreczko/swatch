 
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

bool AbstractState::isEnabled() const
{
  return mEnabled;
}


const std::string& AbstractState::getState() const
{
  return mState;
}


const std::vector<const Functionoid*>& AbstractState::getActions() const
{
  return mActions;
}


AbstractState::AbstractState() :
  mEnabled(true)
{
}


ActionableObject::State::State() : 
  AbstractState(),
  mFSM(NULL)
{
}

const StateMachine* ActionableObject::State::getEngagedFSM() const
{
  return mFSM;
}


//------------------------------------------------------------------------------------

ActionableObject::ActionableObject( const std::string& aId ) :
  MonitorableObject( aId  ),
  mState()
{
}


//------------------------------------------------------------------------------------

ActionableObject::~ActionableObject()
{
}

//------------------------------------------------------------------------------------

CommandSequence& ActionableObject::getCommandSequence( const std::string& aId )
{
  try {
    return *(mCommandSequences.at( aId ));
  } catch ( const std::out_of_range& e ) {
    throw CommandSequenceNotFoundInActionableObject( "Unable to find CommandSequence with ID '" + aId + "' in object '" + getPath() + "'" );
  }
}


Command& ActionableObject::getCommand( const std::string& aId )
{
  try {
    return *(mCommands.at( aId ));
  } catch ( const std::out_of_range& e ) {
    throw CommandNotFoundInActionableObject( "Unable to find Command with ID '" + aId + "' in object '" + getPath() + "'" );
  }
}


StateMachine& ActionableObject::getStateMachine( const std::string& aId )
{
  try {
    return *(mFSMs.at( aId ));
  } catch ( const std::out_of_range& e ) {
    throw StateMachineNotFoundInActionableObject( "Unable to find state machine with ID '" + aId + "' in object '" + getPath() + "'");
  }
}


//------------------------------------------------------------------------------------

std::set<std::string> ActionableObject::getCommandSequences() const
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

CommandSequence& ActionableObject::registerCommandSequence( const std::string& aId, const std::string& aFirstCommandId, const std::string& aFirstCommandAlias)
{ 
  if (mCommandSequences.count(aId)) {
    throw CommandSequenceAlreadyExistsInActionableObject( "CommandSequence With ID '"+aId+"' already exists" );
  }
  CommandSequence* lSequence = new CommandSequence(aId, *this, aFirstCommandId, aFirstCommandAlias);
  mCommandSequences.insert( std::make_pair( aId , lSequence ) );
  return *lSequence;
}


CommandSequence& ActionableObject::registerCommandSequence( const std::string& aId, Command& aFirstCommand, const std::string& aFirstCommandAlias)
{ 
  if (mCommandSequences.count(aId)) {
    throw CommandSequenceAlreadyExistsInActionableObject( "CommandSequence With ID '"+aId+"' already exists" );
  }
  CommandSequence* lSequence = new CommandSequence(aId, *this, aFirstCommand, aFirstCommandAlias);
  mCommandSequences.insert( std::make_pair( aId , lSequence ) );
  return *lSequence;
}


Command& ActionableObject::registerFunctionoid( const std::string& aId , Command* aCommand )
{
  if (mCommands.count(aId)){
    delete aCommand;
    throw CommandAlreadyExistsInActionableObject( "Command With ID '"+aId+"' already exists" );
  }
  this->addObj(aCommand);
  mCommands.insert( std::make_pair( aId , aCommand ) );
  return *aCommand;
}


StateMachine& ActionableObject::registerStateMachine( const std::string& aId, const std::string& aInitialState, const std::string& aErrorState ) {
  if (mFSMs.count(aId))
    throw StateMachineAlreadyExistsInActionableObject( "State machine With ID '"+aId+"' already exists" );

  StateMachine* lFSM = new StateMachine(aId, *this, aInitialState, aErrorState);
  mFSMs.insert( std::make_pair( aId , lFSM ) );
  return *lFSM;
}


ActionableObject::State ActionableObject::getState() const {
  boost::lock_guard<boost::mutex> lGuard(mMutex);
  return mState;
}


void ActionableObject::engageStateMachine(const std::string& aFSM) {
  boost::lock_guard<boost::mutex> lGuard(mMutex);
  
  // Throw if currently in other state machine
  if(mState.getEngagedFSM() != NULL)
    throw ResourceInWrongStateMachine("Cannot engage other state machine; resource '"+getPath()+"' currently in state machine '"+mState.getEngagedFSM()->getId()+"'");

  const StateMachine& lOp = getStateMachine(aFSM);
  mState.mFSM = & lOp;
  mState.mState = lOp.getInitialState();
}


//------------------------------------------------------------------------------------


void ActionableObject::Deleter::operator ()(Object* aObject) {
  if(ActionableObject* lActionableObj = dynamic_cast<ActionableObject*>(aObject))
  {
    LOG(swatch::logger::kNotice) << "ActionableObject deleter being called on object '" << aObject->getPath() << "'";

    lActionableObj->disableActions();

    //TODO (low-ish priority): Eventually replace this "spinning" do-loop with a more efficient implementation based on ActionableObject/Functionoid methods that use conditional variables behind-the-scenes 
    do {
    } while ( ! lActionableObj->getState().getActions().empty() );

    LOG(swatch::logger::kNotice) << "ActionableObject deleter now thinks that object '" << aObject->getPath() << "' has finished all commands";
    
    delete lActionableObj;
  }
  else{
    LOG(swatch::logger::kWarning) << "ActionableObject::Deleter being used on object '" << aObject->getPath() << "' of type '" << aObject->getTypeName() << "' that doesn't inherit from ActionableObject";
    delete aObject;
  }
}


void ActionableObject::disableActions(){
  boost::lock_guard<boost::mutex> lGuard(mMutex);
  mState.mEnabled = false;
}



ActionableObject::BusyGuard::BusyGuard(ActionableObject& aResource, const Functionoid& aFunctionoid, const BusyGuard* aOuterGuard) : 
  mResource(aResource),
  mAction(aFunctionoid),
  mOuterGuard(aOuterGuard)
{
  const boost::unique_lock<boost::mutex> lGuard(aResource.mMutex);
  initialise(lGuard);
}


ActionableObject::BusyGuard::BusyGuard(ActionableObject& aResource, const boost::unique_lock<boost::mutex>& aLockGuard, const Functionoid& aFunctionoid, const BusyGuard* aOuterGuard) : 
  mResource(aResource),
  mAction(aFunctionoid),
  mOuterGuard(aOuterGuard)
{
  initialise(aLockGuard);
}


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
    else if ( mResource.mState.mActions.empty() )
      throw WrongBusyGuard( "Outer BusyGuard used (resource: '"+mResource.getPath()+"', action: '"+mOuterGuard->mAction.getId()+"'), but resource not busy");
    else if ( &mOuterGuard->mAction != mResource.mState.mActions.back() )
      throw WrongBusyGuard( "Outer BusyGuard (resource: '"+mResource.getPath()+"', action: '"+mOuterGuard->mAction.getId()+"') is not for current action '"+mResource.mState.mActions.back()->getId()+"'" );
  }

  // 0) Check that this this action isn't already running
  if (std::count(mResource.mState.mActions.begin(), mResource.mState.mActions.end(), &mAction) > 0 )
    throw ActionableObjectIsBusy( "Action '"+mAction.getId()+"' is already running on resource '"+mResource.getPath()+"'" );

  // 1) For transitions, check that state machine is engaged, and we're in the right state
  if (const StateMachine::Transition* t = dynamic_cast<const StateMachine::Transition*>(&mAction))
  {
    if( mResource.mState.mFSM != & t->getStateMachine())
      throw ResourceInWrongState("Resource '"+mResource.getPath()+"' is not yet engaged in state machine '"+t->getStateMachine().getId()+"'");
    else if ( mResource.mState.mState != t->getStartState() )
      throw ResourceInWrongState("Resource '"+mResource.getPath()+"' is in state '"+mResource.mState.mState+"'; transition '"+t->getId()+"' cannot be run");
  }
  
  // 2) Claim the resource if free; else throw if can't get sole control of it
  if ( mResource.mState.mEnabled && ( (mOuterGuard != NULL) || mResource.mState.mActions.empty() ) )
  {
    LOG(swatch::logger::kInfo) << "'" << mResource.getPath() << "' : Starting action '" << mAction.getId() << "'";
    mResource.mState.mActions.push_back(&mAction);
  }
  else
  {
    std::ostringstream oss;
    oss << "Could not run action '" << mAction.getId() << "' on resource '" << mResource.getPath() << "'. ";

    if ( ! mResource.mState.mActions.empty() )
      oss << "Resource currently busy running functionoid '" << mResource.mState.mActions.back()->getId() << "'.";
    else
      oss << "Actions currently disabled on this resource.";

   LOG(swatch::logger::kNotice) << oss.str();
   throw ActionableObjectIsBusy(oss.str());
  } 
}




ActionableObject::BusyGuard::~BusyGuard()
{  
  boost::lock_guard<boost::mutex> lGuard(mResource.mMutex);
  LOG(swatch::logger::kInfo) << "'" << mResource.getPath() << "' : Finished action '" << mAction.getId() << "'";
  if ( (!mResource.mState.mActions.empty()) && (&mAction == mResource.mState.mActions.back()))
  {
    mResource.mState.mActions.pop_back();
    
    // In case this was a transition, also update object's current state
    if (const StateMachine::Transition* t = dynamic_cast<const StateMachine::Transition*>(&mAction))
    {
      if(t->getStatus().getState() == ActionStatus::kError)
        mResource.mState.mState = t->getStateMachine().getErrorState();
      else
        mResource.mState.mState = t->getEndState();
    }
  }
  else
  {
    size_t lNrActions = mResource.mState.mActions.size();
    const std::string activeFuncId(lNrActions > 0 ? "NULL" : "'" + mResource.mState.mActions.back()->getId() + "' (innermost of "+boost::lexical_cast<std::string>(lNrActions)+")");
    LOG(swatch::logger::kError) << "unexpected active functionoid " << activeFuncId << "  in BusyGuard destructor for resource '" << mResource.getPath() << "', functionoid '" << mAction.getId() << "'";
  }
}


}
}
