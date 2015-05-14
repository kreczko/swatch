#include "swatch/core/ConfigSequence.hpp"
#include "swatch/core/ActionableObject.hpp"
//#include "toolbox/ConfigSequence/exception/Exception.h"

#include <xdata/String.h>

namespace swatch {
namespace core {

ConfigSequence::ConfigSequence( const std::string& aId ) :
  Functionoid( aId ),
  mTables( NULL )
{
  
}

ConfigSequence::~ConfigSequence() {
  if( mTables ) delete mTables;
}

void ConfigSequence::exec()
{
  for( std::deque< Command* >::iterator lIt = mCommands.begin() ; lIt != mCommands.end() ; ++lIt )
  {
    (**lIt).exec();
  }
}

void ConfigSequence::reset()
{
  for( std::deque< Command* >::iterator lIt = mCommands.begin() ; lIt != mCommands.end() ; ++lIt )
  {
    (**lIt).reset();
  }
}

std::set< std::string > ConfigSequence::getParams()
{
  std::set< std::string > lKeys, lAllKeys;
  for( std::deque< Command* >::iterator lIt( mCommands.begin()) ; lIt != mCommands.end() ; ++lIt )
  {
    lKeys = (**lIt).getParams().keys();
    lAllKeys.insert( lKeys.begin() , lKeys.end() );
  }
  return lAllKeys;
}

const std::deque<std::string>& ConfigSequence::getTables()
{
  if( !mTables ) setTables();
  return *mTables;
}

ConfigSequence& ConfigSequence::run( Command* aCommand )
{
  mCommands.push_back( aCommand );
  return *this;
}

ConfigSequence& ConfigSequence::then ( Command* aCommand )
{
  return run( aCommand );
}

ConfigSequence& ConfigSequence::operator() ( Command* aCommand )
{
  return run( aCommand );
}


ConfigSequence& ConfigSequence::run( const std::string& aCommand )
{
  ActionableObject* lParent( getParent<ActionableObject>()  );
  mCommands.push_back( lParent->getCommand( aCommand ) );
  return *this;
}

ConfigSequence& ConfigSequence::then ( const std::string& aCommand )
{
  return run( aCommand );
}

ConfigSequence& ConfigSequence::operator() ( const std::string& aCommand )
{
  return run( aCommand );
}



} /* namespace core */
} /* namespace swatch */
