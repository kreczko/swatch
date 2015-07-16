#include "swatch/core/GateKeeper.hpp"
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/ActionableObject.hpp"
#include "swatch/core/XParameterSet.hpp"
#include "swatch/core/ThreadPool.hpp"

#include <iostream>

//#include "swatch/logger/Log.hpp"

#include <boost/foreach.hpp>
//#include "toolbox/CommandSequence/exception/Exception.h"

#include <xdata/String.h>

namespace swatch {
namespace core {

CommandSequence::CommandSequence( const std::string& aId ) :
  Functionoid( aId ),
  mTables( NULL ),
  mCommands(),
  mIt( mCommands.end() ),
  mGateKeeper( NULL ),
  mCachedParameters(),
  mUpdateTime() ,
  mStatus( kDone )
{
  
}

CommandSequence::~CommandSequence() {
  if( mTables ) delete mTables;
}

bool CommandSequence::precondition()
{
  return true;
}


// Command& CommandSequence::getCommand( boost::any& aCommand ) const
// {
//   try{
//     return *(boost::any_cast< Command* >( aCommand )); //boost::any returns null pointer, only if original object is a pointer, not the requested object type
//   }catch( const boost::bad_any_cast& ){
// 
//     ActionableObject* lParent( const_cast<CommandSequence*>( this )->getParent<ActionableObject>() ); // We promise to be nicely behaved... 
//     if( !lParent ) throw NoParentDefined( "Tried to evaluate the parent when no parent has yet been defined" );
//   
//     Command* lCommand( lParent->getCommand( boost::any_cast< std::string& >( aCommand ) ) ); // getCommand throws if not found, so no NULL check necessary
//     aCommand = lCommand;
//     return *lCommand;
//   }
// }

Command& CommandSequence::getCommand( Command* aCommand ) const
{
  return *aCommand;
}


void CommandSequence::UpdateParameterCache()
{
  mCachedParameters.clear();
  mCachedParameters.reserve( mCommands.size() );

  for( tCommandVector::iterator lIt( mCommands.begin()) ; lIt != mCommands.end() ; ++lIt )
  {
    Command& lCommand( getCommand( *lIt ) );
    ReadOnlyXParameterSet lParams( lCommand.getDefaultParams() );

    std::set< std::string > lKeys( lParams.keys() );
    for( std::set< std::string >::iterator lIt2( lKeys.begin() ); lIt2!=lKeys.end(); ++lIt2 )
    {
      GateKeeper::tParameter lData( mGateKeeper->get( getPath() , lCommand.getId() , *lIt2 , getTables() ) );
      if ( lData.get() != NULL )
      {
        lParams.erase(*lIt2);
        lParams.adopt( *lIt2 , lData );
      }
    }
    mCachedParameters.push_back( lParams );
  }
}


void CommandSequence::exec( const bool& aUseThreadPool )
{
  
  if( !mGateKeeper ) throw NoGateKeeperDefined( "No GateKeeper Defined" );

  // Is our cache of parameters up to date?
  boost::posix_time::ptime lUpdateTime( mGateKeeper->lastUpdated() );
  if( mUpdateTime != lUpdateTime )
  {
    UpdateParameterCache();
    mUpdateTime = lUpdateTime; // We are up to date :)
  }
  

  if ( aUseThreadPool){
    ThreadPool& pool = ThreadPool::getInstance();
    pool.addTask<CommandSequence>( this , &CommandSequence::runCode );
  }
  else{
    // otherwise execute in same thread
    this->runCode();
  }
}

void CommandSequence::threadless_exec()
{
  exec( false );
}


void CommandSequence::runCode( ) {
  try {
    
    tParameterSets::iterator lIt( mCachedParameters.begin() );
    mStatus = kRunning;

    {
      boost::unique_lock<boost::mutex> lock( mIteratorMutex );
      mIt = mCommands.begin();
    }  

    while( true ) 
    {
      boost::unique_lock<boost::mutex> lock( mIteratorMutex );
      if( mIt == mCommands.end() )
      {
        mStatus = kDone;
        return;
      }

      Command& lCommand( getCommand( *mIt ) );
      lCommand.exec( *lIt , false ); // False = run the commands in this thread!

      if( lCommand.getState() != Command::kDone )
      {
        mStatus = kFailed;
        return;
      }

      ++mIt;
      ++lIt;
    }

    
  } catch (const std::exception& e) {
//     this->setError( "An exception occured in Command::code(): " + std::string(e.what()));
    std::cout << "An exception occured in Command::code(): " << e.what() << std::endl;
    mStatus = kFailed;
  }
}


void CommandSequence::reset()
{

  boost::unique_lock<boost::mutex> lock( mIteratorMutex );

  for( mIt = mCommands.begin() ; mIt != mCommands.end() ; ++mIt )
  {
    Command& lCommand( getCommand( *mIt ) );
    lCommand.reset();
  }
}

// std::set< std::string > CommandSequence::getParams()
// {
//   std::set< std::string > lKeys, lAllKeys;
//   for( tCommandVector::iterator lIt( mCommands.begin()) ; lIt != mCommands.end() ; ++lIt )
//   {
//     Command& lCommand( getCommand( *lIt ) );
//     lKeys = lCommand.getDefaultParams().keys();
//     for( std::set< std::string >::iterator lIt2( lKeys.begin() ); lIt2!=lKeys.end(); ++lIt2 )
//     {
//       lAllKeys.insert( id() + "." + lCommand.id() + "." + *lIt2 );
//     }
//   }
//   return lAllKeys;
// }

const std::vector<std::string>& CommandSequence::getTables()
{
  if( !mTables ) mTables = setTables();
  return *mTables;
}

CommandSequence& CommandSequence::run( Command* aCommand )
{
  mCommands.push_back( aCommand );
  return *this;
}

CommandSequence& CommandSequence::then ( Command* aCommand )
{
  return run( aCommand );
}

CommandSequence& CommandSequence::operator() ( Command* aCommand )
{
  return run( aCommand );
}


CommandSequence& CommandSequence::run( const std::string& aCommand )
{
//   mCommands.push_back( aCommand );
//   return *this;
  ActionableObject* lParent( getParent<ActionableObject>()  );
  return run( & lParent->getCommand( aCommand ) );
}

CommandSequence& CommandSequence::then ( const std::string& aCommand )
{
  return run( aCommand );
}

CommandSequence& CommandSequence::operator() ( const std::string& aCommand )
{
  return run( aCommand );
}



const Command* CommandSequence::getCurrentCommand()
{
  boost::unique_lock<boost::mutex> lock( mIteratorMutex );
  if (mIt == mCommands.end() ) return NULL;
  return *mIt;
}

CommandSequence::Status CommandSequence::getStatus() const
{
  return mStatus;
}


float CommandSequence::getProgress()
{
  boost::unique_lock<boost::mutex> lock( mIteratorMutex );
  uint32_t lStep = mIt - mCommands.begin();
  float lProgress = (100. * lStep) + getProgress();
  return lProgress / mCommands.size();
}


std::vector< const xdata::Serializable* > CommandSequence::getResults()
{
  std::vector< const xdata::Serializable* > lRet;
  lRet.reserve( mCommands.size() );

    
  boost::unique_lock<boost::mutex> lock( mIteratorMutex );
  for( tCommandVector::iterator lIt( mCommands.begin()) ; lIt != mIt ; ++lIt )
  {
    Command& lCommand( getCommand( *lIt ) );
    lRet.push_back( lCommand.getStatus().getResult() );
  }
  return lRet;
}



void CommandSequence::setGateKeeper( GateKeeper* aGateKeeper )
{
  mGateKeeper = aGateKeeper;
}

// std::string CommandSequence::mCommandSequenceComplete = std::string( "CommandSequence complete" );

} /* namespace core */
} /* namespace swatch */
