
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

ConfigSequence* ActionableObject::getConfigSequence( const std::string& aId )
{
  try {
    return mConfigSequences.at( aId );
  } catch ( const std::out_of_range& e ) {
    throw ConfigSequenceNotFoundInActionableObject( "Unable to find ConfigSequence with Id '"+aId+"'" );
  }
  return NULL;
}

Command* ActionableObject::getCommand( const std::string& aId )
{
  try {
    return mCommands.at( aId );
  } catch ( const std::out_of_range& e ) {
    throw CommandNotFoundInActionableObject( "Unable to find Command with Id '"+aId+"'" );
  }
  return NULL;
}

Operation* ActionableObject::getOperation( const std::string& aId )
{
  try {
    return mOperations.at( aId );
  } catch ( const std::out_of_range& e ) {
    throw OperationNotFoundInActionableObject( "Unable to find Operation with Id '"+aId+"'" );
  }
  return NULL;
}

//------------------------------------------------------------------------------------

std::set<std::string> ActionableObject::getConfigSequences() const
{
  std::set<std::string> lNames;
  BOOST_FOREACH( tConfigSequenceMap::value_type p, mConfigSequences) {
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

void ActionableObject::Register( const std::string& aId , ConfigSequence* aConfigSequence )
{
  if (mConfigSequences.count(aId)) throw ConfigSequenceAlreadyExistsInActionableObject( "ConfigSequence With Id '"+aId+"' Already Exists" );
  this->addObj(aConfigSequence);
  mConfigSequences.insert( std::make_pair( aId , aConfigSequence ) );
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
