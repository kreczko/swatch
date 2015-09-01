#include "swatch/core/CommandSequence.hpp"


#include <iostream>
#include <vector>

#include "swatch/core/ActionableObject.hpp"
#include "swatch/core/Command.hpp"
#include "swatch/core/GateKeeper.hpp"
#include "swatch/core/XParameterSet.hpp"
#include "swatch/core/ThreadPool.hpp"
#include "swatch/logger/Log.hpp"


namespace swatch {
namespace core {

std::ostream& operator<<(std::ostream& out, swatch::core::CommandSequence::State s) {
  switch (s) {
    case swatch::core::CommandSequence::kInitial   : out << "Initial"; break;
    case swatch::core::CommandSequence::kScheduled : out << "Scheduled"; break;
    case swatch::core::CommandSequence::kRunning   : out << "Running"; break;
    case swatch::core::CommandSequence::kWarning   : out << "Warning"; break;
    case swatch::core::CommandSequence::kError     : out << "Error"; break;
    case swatch::core::CommandSequence::kDone      : out << "Done"; break;
    default : out << "Unknown value of swatch::core::CommandSequence::Status enum"; 
  }
  return out;
}
  
  
CommandSequence::CommandSequence( const std::string& aId, ActionableObject& aResource, const std::string& aFirstCommandId, const std::string& aFirstCommandAlias) :
  Functionoid( aId ),
  mResource(aResource),
  mCommands(),
  mCachedParameters(),
  mParamUpdateTime() ,
  mState( kInitial ),
  mCommandIt( mCommands.end() )
{
  aResource.addObj(this);

  run(aFirstCommandId, aFirstCommandAlias);
}


CommandSequence::CommandSequence( const std::string& aId, ActionableObject& aResource, Command& aFirstCommand, const std::string& aFirstCommandAlias) :
  Functionoid( aId ),
  mResource(aResource),
  mCommands(),
  mCachedParameters(),
  mParamUpdateTime() ,
  mState( kInitial ),
  mCommandIt( mCommands.end() )
{
  aResource.addObj(this);
  
  run(aFirstCommand, aFirstCommandAlias);
}


CommandSequence::~CommandSequence() {
}


CommandSequence& CommandSequence::run( Command& aCommand, const std::string& aAlias )
{
  if( &aCommand.getResource() != &mResource )
  {
    throw InvalidResource("Cannot add command '" + aCommand.getId() + "' (resource: "+aCommand.getResource().getPath()+") ' to sequence of resource '" + mResource.getPath() + "'");
  }
  mCommands.push_back( std::make_pair(&aCommand, aAlias) );
  mCommandIt = mCommands.end();
  return *this;
}


CommandSequence& CommandSequence::then ( Command& aCommand, const std::string& aAlias )
{
  return run( aCommand, aAlias );
}


CommandSequence& CommandSequence::operator() ( Command& aCommand, const std::string& aAlias )
{
  return run( aCommand, aAlias );
}


CommandSequence& CommandSequence::run( const std::string& aCommand, const std::string& aAlias )
{
  Command& lCommand = mResource.getCommand( aCommand );
  mCommands.push_back( std::make_pair(&lCommand, aAlias) );
  mCommandIt = mCommands.end();
  return *this;
}


CommandSequence& CommandSequence::then ( const std::string& aCommand, const std::string& aAlias )
{
  return run( aCommand, aAlias );
}


CommandSequence& CommandSequence::operator() ( const std::string& aCommand, const std::string& aAlias )
{
  return run( aCommand, aAlias );
}


const std::vector< std::pair<Command*, std::string> >& CommandSequence::getCommands() const 
{
  return mCommands;
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




void CommandSequence::exec(const GateKeeper& aGateKeeper, const bool& aUseThreadPool )
{
  // 1) Request sole control of the resource
  boost::shared_ptr<ActionableObject::BusyGuard> lActionGuard(new ActionableObject::BusyGuard(*getParent<ActionableObject>(),*this));

// FIXME: Re-implement parameter cache at some future date; disabled by Tom on 28th August, since ...
//        ... current logic doesn't work correctly with different gatekeepers - need to change to ...
//        ... updating cache if either gatekeeper filename/runkey different or cache update timestamp older than gatekeeper.lastUpdated 
//  // Is our cache of parameters up to date?
//  boost::posix_time::ptime lUpdateTime( aGateKeeper.lastUpdated() );
//  if( mParamUpdateTime != lUpdateTime )
//  {
//    updateParameterCache(aGateKeeper);
//    mParamUpdateTime = lUpdateTime; // We are up to date :)
//  }
  
  // 2) Reset the status of this sequence's state variables
  {
    boost::unique_lock<boost::mutex> lock( mMutex );

    // Extract parameters first, so that don't change state if a parameter is missing
    std::vector<std::pair<std::string,std::string> > missingParams;
    extractParameters(aGateKeeper, mCachedParameters, missingParams, true);

    mState = kInitial;
    mCommandIt = mCommands.end();
    mStatusOfCompletedCommands.clear();
    mStatusOfCompletedCommands.reserve(mCommands.size());
  }  

  // 3) Execute the command
  if ( aUseThreadPool){
    boost::unique_lock<boost::mutex> lock(mMutex);
    mState = kScheduled;
    
    ThreadPool& pool = ThreadPool::getInstance();
    pool.addTask<CommandSequence, ActionableObject::BusyGuard>( this , &CommandSequence::runCommands, lActionGuard);
  }
  else{
    // otherwise execute in same thread
    this->runCommands(lActionGuard);
  }
}


CommandSequence::State CommandSequence::getState() const 
{
  boost::unique_lock<boost::mutex> lock(mMutex);
  return mState;
}


CommandSequenceStatus CommandSequence::getStatus() const 
{
  boost::unique_lock<boost::mutex> lock(mMutex);
  
  float runningTime = 0.0;
  switch (mState) {
    case kInitial : 
    case kScheduled :
      break;
    default :
      boost::posix_time::ptime endTime;
      if (mState == kRunning)
        endTime = boost::posix_time::microsec_clock::universal_time();
      else
        endTime = mExecEndTime;
      
      boost::posix_time::time_duration duration = endTime - mExecStartTime;
      runningTime = duration.total_seconds();
      runningTime += double(duration.fractional_seconds()) / pow(10.0, duration.num_fractional_digits());

      break;
  }
  
  const Command* currentCommand =  ( (mCommandIt == mCommands.end()) ? NULL : mCommandIt->first);
  
  return CommandSequenceStatus(getPath(), mState, runningTime, currentCommand, mStatusOfCompletedCommands, mCommands.size());
}


/*
void CommandSequence::reset()
{
  boost::unique_lock<boost::mutex> lock( mIteratorMutex );

  for( mIt = mCommands.begin() ; mIt != mCommands.end() ; ++mIt )
  {
    Command& lCommand( getCommand( *mIt ) );
    lCommand.reset();
  }
}
*/


bool CommandSequence::precondition()
{
  return true;
}


void CommandSequence::runCommands(boost::shared_ptr<ActionableObject::BusyGuard> aGuard)
{
  // 1) Declare that I'm running 
  {
    boost::unique_lock<boost::mutex> lock( mMutex );
    mState = kRunning;
    mCommandIt = mCommands.begin();
    mExecStartTime = boost::posix_time::microsec_clock::universal_time();
  }  
 
  // 2) Run the commands
  try {
    tParameterSets::iterator lIt( mCachedParameters.begin() );
    
    while( true ) 
    {
      mCommandIt->first->exec(aGuard, *lIt , false ); // False = run the commands in this thread!
      //FIXME: Make exec method return CommandStatus to remove any possibility of race condition ?
      CommandStatus status = mCommandIt->first->getStatus();
      
      boost::unique_lock<boost::mutex> lock(mMutex);
      mStatusOfCompletedCommands.push_back(status);

      // Don't execute any more commands if there was an error
      if( status.getState() == Command::kError )
      {
        mState = kError;
        mExecEndTime = boost::posix_time::microsec_clock::universal_time();
        return;
      }

      // Increment the "current command" iterator 
      ++mCommandIt;
      ++lIt;

      // Exit the loop if no more commands remain
      if( mCommandIt == mCommands.end() )
      {
        mState = kDone;
        for(std::vector<CommandStatus>::const_iterator statusIt=mStatusOfCompletedCommands.begin(); statusIt != mStatusOfCompletedCommands.end(); statusIt++)
        {
          if(statusIt->getState() == Command::kWarning)
            mState = kWarning;
        }
        mExecEndTime = boost::posix_time::microsec_clock::universal_time();
        return;
      }
    }
    
  } catch (const std::exception& e) {
//     this->setError( "An exception occured in Command::code(): " + std::string(e.what()));
    std::cout << "An exception occurred in CommandSequence::runCommands(): " << e.what() << std::endl;
    
    boost::unique_lock<boost::mutex> lock( mMutex );
    mState = kError;
    mExecStartTime = boost::posix_time::microsec_clock::universal_time();
  }
  
  // 3) The resource is released by destruction of BusyGuard
}


void CommandSequence::checkForMissingParameters(const GateKeeper& aGateKeeper, std::vector<ReadOnlyXParameterSet>& aParamSets, std::vector<std::pair<std::string,std::string> >& aMissingParams) const
{
  extractParameters(aGateKeeper, aParamSets, aMissingParams, false);
}


void CommandSequence::extractParameters(const GateKeeper& aGateKeeper, std::vector<ReadOnlyXParameterSet>& aParamSets, std::vector<std::pair<std::string,std::string> >& aMissingParams, bool throwOnMissing) const
{
  aParamSets.clear();
  aParamSets.reserve( mCommands.size() );

  aMissingParams.clear();

  for( tCommandVector::const_iterator lIt( mCommands.begin()) ; lIt != mCommands.end() ; ++lIt )
  {
    Command& lCommand( *lIt->first );
    const std::string& lCommandAlias = (lIt->second.empty() ? lCommand.getId() : lIt->second);

    ReadOnlyXParameterSet lParams;
    std::set< std::string > lKeys( lCommand.getDefaultParams().keys() );
    for( std::set< std::string >::iterator lIt2( lKeys.begin() ); lIt2!=lKeys.end(); ++lIt2 )
    {
      GateKeeper::tParameter lData( aGateKeeper.get( getId() , lCommandAlias , *lIt2 , getParent<swatch::core::ActionableObject>()->getGateKeeperTables() ) );
      if ( lData.get() != NULL )
      {
        lParams.adopt( *lIt2 , lData );
      }
      else if (throwOnMissing) {
        std::ostringstream oss;
        oss << "Could not find value of parameter '" << *lIt2 << "' for command with alias '" << lCommandAlias << "' in sequence '" << getId() << "' of resource '" << getParent<swatch::core::ActionableObject>()->getId() << "'";
        LOG(swatch::logger::kError) << oss.str();
        throw ParameterNotFound(oss.str());
      }
      else {
        aMissingParams.push_back(std::make_pair(lCommandAlias, *lIt2));
      }
    }
    aParamSets.push_back( lParams );
  }
}




CommandSequenceStatus::CommandSequenceStatus(const std::string& aPath, CommandSequence::State aState, float aRunningTime, const Command* aCurrentCommand, const std::vector<CommandStatus>& aStatusOfCompletedCommands,  size_t aTotalNumberOfCommands) : 
  mPath(aPath),
  mState(aState),
  mRunningTime(aRunningTime),
  mTotalNumberOfCommands( aTotalNumberOfCommands ),
  mCommandStatuses(aStatusOfCompletedCommands)
{
  if (aCurrentCommand != NULL)
  {
    mCommandStatuses.push_back(aCurrentCommand->getStatus());
  } 
 
  for(auto it = aStatusOfCompletedCommands.begin(); it != aStatusOfCompletedCommands.end(); it++)
    mResults.push_back(it->getResult());
}

const std::string&
CommandSequenceStatus::getSequencePath() const {
  return mPath;
}


CommandSequence::State CommandSequenceStatus::getState() const
{
  return mState;
}


float CommandSequenceStatus::getRunningTime() const
{
  return mRunningTime;
}


float CommandSequenceStatus::getProgress() const
{
  if (mCommandStatuses.empty())
    return 0.0;
  else if (mResults.size() == mTotalNumberOfCommands)
    return 1.0;
  else
    return ( float(mResults.size()) + mCommandStatuses.back().getProgress() ) / float(mTotalNumberOfCommands);
}

size_t
CommandSequenceStatus::getNumberOfCompletedCommands() const {
  return mResults.size();
}

size_t
CommandSequenceStatus::getTotalNumberOfCommands() const {
  return mTotalNumberOfCommands;
}


const std::vector<const xdata::Serializable*>& CommandSequenceStatus::getResults() const
{
  return mResults;
}


const std::vector<CommandStatus>& CommandSequenceStatus::getCommandStatus() const
{
  return mCommandStatuses;
}

} /* namespace core */
} /* namespace swatch */
