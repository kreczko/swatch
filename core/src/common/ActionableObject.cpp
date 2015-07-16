
#include "swatch/core/ActionableObject.hpp"


#include "boost/foreach.hpp"

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

CommandSequence& ActionableObject::registerFunctionoid( const std::string& aId , CommandSequence* aCommandSequence )
{
  if (mCommandSequences.count(aId)) {
    delete aCommandSequence;
    throw CommandSequenceAlreadyExistsInActionableObject( "CommandSequence With ID '"+aId+"' already exists" );
  }
  this->addObj(aCommandSequence);
  mCommandSequences.insert( std::make_pair( aId , aCommandSequence ) );
  return *aCommandSequence;
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


std::pair<bool, const Functionoid *  > ActionableObject::requestControlOfResource(Functionoid const * const aFunctionoid) {
  boost::lock_guard<boost::mutex> lGuard(mMutex);
  if (! mEnabled )
    return std::make_pair(false, (const Functionoid*) NULL);
  else if ( this->mActiveFunctionoid != NULL)
    return std::pair<bool, const Functionoid*>(false, mActiveFunctionoid);
  else {
    mActiveFunctionoid = aFunctionoid;
    return std::pair<bool, const Functionoid*>(true, mActiveFunctionoid);
  }
}


const Functionoid * ActionableObject::releaseControlOfResource(Functionoid const * const aFunctionoid){  
  boost::lock_guard<boost::mutex> lGuard(mMutex);
  if (aFunctionoid == mActiveFunctionoid)
    mActiveFunctionoid = NULL;
  
  return mActiveFunctionoid;
}


}
}
