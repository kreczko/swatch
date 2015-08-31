 
#include "swatch/core/ActionableObject.hpp"


#include "boost/foreach.hpp"

#include "swatch/core/Command.hpp"
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/Operation.hpp"
#include "swatch/core/Utilities.hpp"
#include "swatch/logger/Log.hpp"


using namespace std;

namespace swatch {
namespace core {

//------------------------------------------------------------------------------------

ActionableObject::ActionableObject( const std::string& aId ) :
  MonitorableObject( aId  ),
  mActiveFunctionoid(NULL),
  mEnabled(true)
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


Operation& ActionableObject::getOperation( const std::string& aId )
{
  try {
    return *(mOperations.at( aId ));
  } catch ( const std::out_of_range& e ) {
    throw OperationNotFoundInActionableObject( "Unable to find Operation with ID '" + aId + "' in object '" + getPath() + "'");
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

std::set<std::string> ActionableObject::getOperations() const
{
  std::set<std::string> lNames;
  BOOST_FOREACH( tOperationMap::value_type p, mOperations) {
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


Operation& ActionableObject::registerFunctionoid( const std::string& aId , Operation* aOperation ) {
  if (mOperations.count(aId)){
    delete aOperation;
    throw OperationAlreadyExistsInActionableObject( "Operation With ID '"+aId+"' already exists" );
  }
  this->addObj(aOperation);
  mOperations.insert( std::make_pair( aId , aOperation ) );
  return *aOperation;
}


bool ActionableObject::isEnabled() const {
  boost::lock_guard<boost::mutex> lGuard(mMutex);
  return mEnabled;
}


const Functionoid* ActionableObject::getActiveFunctionoid() const {
  boost::lock_guard<boost::mutex> lGuard(mMutex);
  return mActiveFunctionoid;
}

//------------------------------------------------------------------------------------


void ActionableObject::Deleter::operator ()(Object* aObject) {
  if(ActionableObject* lActionableObj = dynamic_cast<ActionableObject*>(aObject))
  {
    LOG(swatch::logger::kNotice) << "ActionableObject deleter being called on object '" << aObject->getPath() << "'";

    lActionableObj->disableActions();

    //TODO (low-ish priority): Eventually replace this "spinning" do-loop with a more efficient implementation based on ActionableObject/Functionoid methods that use conditional variables behind-the-scenes 
    do {
    } while ( lActionableObj->getActiveFunctionoid() != NULL );

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
  mEnabled = false;
}



ActionableObject::BusyGuard::BusyGuard(ActionableObject& aResource, const Functionoid& aFunctionoid) : 
  mResource(aResource),
  mAction(aFunctionoid)
{
  boost::lock_guard<boost::mutex> lGuard(aResource.mMutex);
  LOG(swatch::logger::kDebug) << "ActionableObject::BusyGuard CTOR for resource '" << mResource.getPath() << "', functionoid '" << mAction.getId() <<"'";

  // Claim the resource if free; else throw if can't get sole control of it
  if ( aResource.mEnabled && ( aResource.mActiveFunctionoid == NULL ) )
    aResource.mActiveFunctionoid = &aFunctionoid;
  else
  {
    std::ostringstream oss;
    oss << "Could not run action '" << aFunctionoid.getId() << "' on resource '" << aResource.getPath() << "'. ";

    if ( aResource.mActiveFunctionoid != NULL )
      oss << "Resource currently busy running functionoid '" << aResource.mActiveFunctionoid->getId() << "'.";
    else
      oss << "Actions currently disabled on this resource.";

   LOG(swatch::logger::kNotice) << oss.str();
   throw ActionableObjectIsBusy(oss.str());
  } 
}


ActionableObject::BusyGuard::~BusyGuard()
{
  boost::lock_guard<boost::mutex> lGuard(mResource.mMutex);
  LOG(swatch::logger::kDebug) << "ActionableObject::BusyGuard DTOR for resource '" << mResource.getPath() << "', functionoid '" << mAction.getId() <<"'";
  if (&mAction == mResource.mActiveFunctionoid)
    mResource.mActiveFunctionoid = NULL;
  else
  {
    const std::string activeFuncId(mResource.mActiveFunctionoid == NULL ? "NULL" : "'" + mResource.mActiveFunctionoid->getId() + "'");
    LOG(swatch::logger::kError) << "Unexpected active functionoid " << activeFuncId << " in ActionGuard destructor for resource '" << mResource.getPath() << "', functionoid '" << mAction.getId() << "'";
  }
}


}
}
