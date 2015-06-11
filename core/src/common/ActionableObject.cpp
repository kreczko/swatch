
#include "swatch/core/ActionableObject.hpp"
#include <boost/foreach.hpp>

using namespace std;

namespace swatch {
namespace core {

//------------------------------------------------------------------------------------

ActionableObject::ActionableObject( const std::string& aId ) :
    MonitorableObject( aId  )
{
}

ActionableObject::ActionableObject( const std::string& aId , const XParameterSet& aParams ) :
    MonitorableObject( aId , aParams )
{
}

//------------------------------------------------------------------------------------

ActionableObject::~ActionableObject()
{
}

//------------------------------------------------------------------------------------

CommandSequence* ActionableObject::getCommandSequence( const std::string& aId )
{
  try {
    return mCommandSequences.at( aId );
  } catch ( const std::out_of_range& e ) {
    throw CommandSequenceNotFoundInActionableObject( "Unable to find CommandSequence with Id '"+aId+"'" );
  }
  return NULL; //Stop the compiler complaining
}

Command* ActionableObject::getCommand( const std::string& aId )
{
  try {
    return mCommands.at( aId );
  } catch ( const std::out_of_range& e ) {
    throw CommandNotFoundInActionableObject( "Unable to find Command with Id '"+aId+"'" );
  }
  return NULL; //Stop the compiler complaining
}

Operation* ActionableObject::getOperation( const std::string& aId )
{
  try {
    return mOperations.at( aId );
  } catch ( const std::out_of_range& e ) {
    throw OperationNotFoundInActionableObject( "Unable to find Operation with Id '"+aId+"'" );
  }
  return NULL; //Stop the compiler complaining
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

void ActionableObject::Register( const std::string& aId , CommandSequence* aCommandSequence )
{
  if (mCommandSequences.count(aId)) throw CommandSequenceAlreadyExistsInActionableObject( "CommandSequence With Id '"+aId+"' Already Exists" );
  this->addObj(aCommandSequence);
  mCommandSequences.insert( std::make_pair( aId , aCommandSequence ) );
}

void ActionableObject::Register( const std::string& aId , Command* aCommand )
{
  if (mCommands.count(aId)) throw CommandAlreadyExistsInActionableObject( "Command With Id '"+aId+"' Already Exists" );
  this->addObj(aCommand);
  mCommands.insert( std::make_pair( aId , aCommand ) );
}

void ActionableObject::Register( const std::string& aId , Operation* aOperation ) {
  if (mOperations.count(aId)) throw OperationAlreadyExistsInActionableObject( "Operation With Id '"+aId+"' Already Exists" );
  this->addObj(aOperation);
  mOperations.insert( std::make_pair( aId , aOperation ) );
}

//------------------------------------------------------------------------------------


}
}
