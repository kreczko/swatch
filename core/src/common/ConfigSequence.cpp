
#include "swatch/core/ConfigSequence.hpp"
//#include "toolbox/ConfigSequence/exception/Exception.h"

#include <xdata/String.h>

namespace swatch {
namespace core {

ConfigSequence::ConfigSequence( Object& aSystem ) : 
  mSystem( &aSystem ),
  mProcessor( NULL ),
  mHardwareType( NULL ),
  mSystemId( NULL ),
  mComponentId( NULL )
{
  
}

ConfigSequence::ConfigSequence( Object& aSystem , Object& aProcessor , const std::string& aHardwareType ) : 
  mSystem( &aSystem ),
  mProcessor( &aProcessor ),
  mHardwareType( new std::string( aHardwareType ) ),
  mSystemId( NULL ),
  mComponentId( NULL )
{
  
}

ConfigSequence::~ConfigSequence() {
  if( mHardwareType ){ delete mHardwareType; }
  if( mSystemId ){ delete mSystemId; }
  if( mComponentId ){ delete mComponentId; }
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


void ConfigSequence::run( Command* aCommand )
{
  mCommands.push_back( aCommand );
}


const std::string* ConfigSequence::getHardwareType()
{
  return mHardwareType;
}

const std::string* ConfigSequence::getSystemId()
{
  if( !mSystemId )
  {
    mSystemId = new std::string( mSystem->path() );
  }
  return mSystemId;
}

const std::string* ConfigSequence::getComponentId()
{
  if( mProcessor and !mComponentId )
  {
    mComponentId = new std::string( mProcessor->path() );
  }
  return mComponentId;
}

} /* namespace core */
} /* namespace swatch */
