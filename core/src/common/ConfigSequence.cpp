#include "swatch/core/GateKeeper.hpp"
#include "swatch/core/ConfigSequence.hpp"
#include "swatch/core/ActionableObject.hpp"
//#include "toolbox/ConfigSequence/exception/Exception.h"

#include <xdata/String.h>

namespace swatch {
namespace core {

ConfigSequence::ConfigSequence( const std::string& aId ) :
  Functionoid( aId ),
  mTables( NULL ),
  mCommands(),
  mIt( mCommands.end() ),
  mGateKeeper( NULL )
{
  
}

ConfigSequence::~ConfigSequence() {
  if( mTables ) delete mTables;
}

void ConfigSequence::configure()
{
  if( !mGateKeeper ) throw NoGateKeeperDefined( "No GateKeeper Defined" );

  for( std::vector< Command* >::iterator lIt( mCommands.begin()) ; lIt != mCommands.end() ; ++lIt )
  {
    XParameterSet lParams( (**lIt).getDefaultParams() );

    std::set< std::string > lKeys( lParams.keys() );
    for( std::set< std::string >::iterator lIt2( lKeys.begin() ); lIt2!=lKeys.end(); ++lIt2 )
    {
      std::string lPath( id() + "." + (**lIt).id() + "." + *lIt2 );
      xdata::Serializable* lData( mGateKeeper->get( lPath , getTables() ) );
      lParams.update( *lIt2 , *lData ); ///TODO : Who has ownership of the xdata::Serializable* ????!
    }

  }
}

void ConfigSequence::exec(const XParameterSet& params)
{
  const XParameterSet merged_params = mergeParametersWithDefaults(params);
  for( mIt = mCommands.begin() ; mIt != mCommands.end() ; ++mIt )
  {
    (**mIt).exec(merged_params);
    if( (**mIt).getStatus() != Command::kDone ) return;
  }
}

void ConfigSequence::reset()
{
  for( mIt = mCommands.begin() ; mIt != mCommands.end() ; ++mIt )
  {
    (**mIt).reset();
  }
}

std::set< std::string > ConfigSequence::getParams()
{
  std::set< std::string > lKeys, lAllKeys;
  for( std::vector< Command* >::iterator lIt( mCommands.begin()) ; lIt != mCommands.end() ; ++lIt )
  {
    lKeys = (**lIt).getDefaultParams().keys();
    for( std::set< std::string >::iterator lIt2( lKeys.begin() ); lIt2!=lKeys.end(); ++lIt2 )
    {
      lAllKeys.insert( id() + "." + (**lIt).id() + "." + *lIt2 );
    }
  }
  return lAllKeys;
}

const std::vector<std::string>& ConfigSequence::getTables()
{
  if( !mTables ) mTables = setTables();
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
  return run( lParent->getCommand( aCommand ) );
}

ConfigSequence& ConfigSequence::then ( const std::string& aCommand )
{
  return run( aCommand );
}

ConfigSequence& ConfigSequence::operator() ( const std::string& aCommand )
{
  return run( aCommand );
}


Command::Status ConfigSequence::getStatus() const
{
  if (mIt == mCommands.end() ) return Command::kDone;
  return (**mIt).getStatus();
}

float ConfigSequence::getProgress() const
{
  if (mIt ==mCommands.end() ) return 100.;
  return (**mIt).getProgress();
}

float ConfigSequence::getOverallProgress() const
{
  uint32_t lStep = mIt - mCommands.begin();
  float lProgress = (100. * lStep) + getProgress();
  return lProgress / mCommands.size();
}

const std::string& ConfigSequence::getProgressMsg() const
{
  if (mIt == mCommands.end() ) return mConfigSequenceComplete;
  return (**mIt).getProgressMsg();
}

const std::string& ConfigSequence::getStatusMsg() const
{
  if (mIt == mCommands.end() ) return mConfigSequenceComplete;
  return (**mIt).getStatusMsg();
}


void ConfigSequence::setGateKeeper( GateKeeper* aGateKeeper )
{
  mGateKeeper = aGateKeeper;
}

std::string ConfigSequence::mConfigSequenceComplete = std::string( "ConfigSequence complete" );

} /* namespace core */
} /* namespace swatch */
