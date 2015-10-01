#include "swatch/core/CommandVec.hpp"


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


CommandVec::CommandVec( const std::string& aId, ActionableObject& aResource) :
  Functionoid( aId ),
  mResource(aResource),
  mCommands(),
  mCachedParameters(),
  mParamUpdateTime() ,
  mState( ActionStatus::kInitial ),
  mCommandIt( mCommands.end() )
{
}


void CommandVec::addCommand(Command& aCommand, const std::string& aNamespace, const std::string& aAlias)
{
  if( &aCommand.getResource() != &mResource )
  {
    throw InvalidResource("Cannot add command '" + aCommand.getId() + "' (resource: "+aCommand.getResource().getPath()+") ' to sequence of resource '" + mResource.getPath() + "'");
  }
  mCommands.push_back( Element(aCommand, aNamespace, aAlias) );
  mCommandIt = mCommands.end();
}


void CommandVec::addCommand(const std::string& aCommand, const std::string& aNamespace, const std::string& aAlias)
{
  Command& lCommand = mResource.getCommand( aCommand );
  mCommands.push_back( Element(lCommand, aNamespace, aAlias) );
  mCommandIt = mCommands.end();
}


CommandVec::~CommandVec() {
}


CommandVec::Element::Element(Command& aCommand, const std::string& aNamespace, const std::string& aAlias) :
  mCmd(&aCommand),
  mAlias(aAlias),
  mNamespace(aNamespace)
{
}


CommandVec::Element::~Element()
{
}


const std::string& CommandVec::Element::getAlias() const
{
  return mAlias;
}

const std::string& CommandVec::Element::getNamespace() const
{
  return mNamespace;
}

const Command& CommandVec::Element::get() const
{
  return *mCmd;
}


const ActionableObject& CommandVec::getResource() const
{
  return mResource;
}


ActionableObject& CommandVec::getResource()
{
  return mResource;
}


size_t CommandVec::size() const
{
  return mCommands.size();
}


CommandVec::const_iterator CommandVec::begin() const
{
  return mCommands.begin();
}


CommandVec::const_iterator CommandVec::end() const
{
  return mCommands.end();
}


std::vector<Command*> CommandVec::getCommands()
{
  std::vector<Command*> lCmds;
  lCmds.reserve(size());
  for(auto lIt=mCommands.begin(); lIt!=mCommands.end(); lIt++)
    lCmds.push_back(lIt->mCmd);
  return lCmds;
}


void CommandVec::exec(const GateKeeper& aGateKeeper, const bool& aUseThreadPool )
{
  boost::shared_ptr<ActionableObject::BusyGuard> lActionGuard(new ActionableObject::BusyGuard(mResource,*this));

  //TODO: Move extraction of parameters from gatekeeper here.
  exec(lActionGuard, aGateKeeper, aUseThreadPool);
}

void CommandVec::exec(const boost::shared_ptr<ActionableObject::BusyGuard>& aBusyGuard, const GateKeeper& aGateKeeper, const bool& aUseThreadPool )
{
  aBusyGuard->check(mResource, *this);

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
  
  // 1) Reset the status of this sequence's state variables
  {
    boost::unique_lock<boost::mutex> lock( mMutex );

    // Extract parameters first, so that don't change state if a parameter is missing
    std::vector<std::pair<std::string,std::string> > missingParams;
    extractParameters(aGateKeeper, mCachedParameters, missingParams, true);

    mState = ActionStatus::kInitial;
    mCommandIt = mCommands.end();
    mStatusOfCompletedCommands.clear();
    mStatusOfCompletedCommands.reserve(mCommands.size());
  }  

  // 2) Execute the command
  if ( aUseThreadPool){
    boost::unique_lock<boost::mutex> lock(mMutex);
    mState = ActionStatus::kScheduled;
    
    ThreadPool& pool = ThreadPool::getInstance();
    pool.addTask<CommandVec, ActionableObject::BusyGuard>( this , &CommandVec::runCommands, aBusyGuard);
  }
  else{
    // otherwise execute in same thread
    this->runCommands(aBusyGuard);
  }
}


ActionStatus::State CommandVec::getState() const 
{
  boost::unique_lock<boost::mutex> lock(mMutex);
  return mState;
}


CommandVecStatus CommandVec::getStatus() const 
{
  boost::unique_lock<boost::mutex> lock(mMutex);
  
  float runningTime = 0.0;
  switch (mState) {
    case ActionStatus::kInitial : 
    case ActionStatus::kScheduled :
      break;
    default :
      boost::posix_time::ptime endTime;
      if (mState == ActionStatus::kRunning)
        endTime = boost::posix_time::microsec_clock::universal_time();
      else
        endTime = mExecEndTime;
      
      boost::posix_time::time_duration duration = endTime - mExecStartTime;
      runningTime = duration.total_seconds();
      runningTime += double(duration.fractional_seconds()) / pow(10.0, duration.num_fractional_digits());

      break;
  }
  
  const Command* currentCommand =  ( (mCommandIt == mCommands.end()) ? NULL : &mCommandIt->get());
  
  return CommandVecStatus(getPath(), mState, runningTime, currentCommand, mStatusOfCompletedCommands, mCommands.size());
}


void CommandVec::runCommands(boost::shared_ptr<ActionableObject::BusyGuard> aGuard)
{
  // 1) Declare that I'm running 
  {
    boost::unique_lock<boost::mutex> lock( mMutex );
    mExecStartTime = boost::posix_time::microsec_clock::universal_time();
    // Finish straight away if there aren't any commands to run
    if( mCommands.empty() )
    {
      mState = ActionStatus::kDone;
      mCommandIt = mCommands.end();
      mExecEndTime = mExecStartTime;
      return;
    }
    else
    {
      mState = ActionStatus::kRunning;
      mCommandIt = mCommands.begin();
    }
  }

  // 2) Run the commands
  try {
    tParameterSets::iterator lIt( mCachedParameters.begin() );
    
    while( true ) 
    {
      {
      LOG(swatch::logger::kNotice) << "'" << getResource().getPath() << "', '" << getId() << "' : Starting running command '" << mCommandIt->mCmd->getId() << "'";
        boost::shared_ptr<ActionableObject::BusyGuard> lCmdGuard( new ActionableObject::BusyGuard(mResource, *mCommandIt->mCmd, aGuard.get()) );
        mCommandIt->mCmd->exec(lCmdGuard, *lIt , false ); // False = run the commands in this thread!
        //FIXME: Make exec method return CommandStatus to remove any possibility of race condition ?
      }
      LOG(swatch::logger::kNotice) << "'" << getResource().getPath() << "', '" << getId() << "' : Finished running command '" << mCommandIt->mCmd->getId() << "'";

      CommandStatus status = mCommandIt->get().getStatus();
      boost::unique_lock<boost::mutex> lock(mMutex);
      mStatusOfCompletedCommands.push_back(status);

      // Don't execute any more commands if there was an error
      if( status.getState() == ActionStatus::kError )
      {
        mState = ActionStatus::kError;
        mExecEndTime = boost::posix_time::microsec_clock::universal_time();
        return;
      }

      // Increment the "current command" iterator 
      ++mCommandIt;
      ++lIt;

      // Exit the loop if no more commands remain
      if( mCommandIt == mCommands.end() )
      {
        mState = ActionStatus::kDone;
        for(std::vector<CommandStatus>::const_iterator statusIt=mStatusOfCompletedCommands.begin(); statusIt != mStatusOfCompletedCommands.end(); statusIt++)
        {
          if(statusIt->getState() == ActionStatus::kWarning)
            mState = ActionStatus::kWarning;
        }
        mExecEndTime = boost::posix_time::microsec_clock::universal_time();
        return;
      }
    }
    
  }
  catch (const std::exception& e)
  {
    std::cout << "An exception occurred in CommandVec::runCommands(): " << e.what() << std::endl;
    
    boost::unique_lock<boost::mutex> lock( mMutex );
    mState = ActionStatus::kError;
    mExecEndTime = boost::posix_time::microsec_clock::universal_time();
  }
  
  // 3) The resource is released by destruction of BusyGuard
}


void CommandVec::checkForMissingParameters(const GateKeeper& aGateKeeper, std::vector<ReadOnlyXParameterSet>& aParamSets, std::vector<std::pair<std::string,std::string> >& aMissingParams) const
{
  extractParameters(aGateKeeper, aParamSets, aMissingParams, false);
}


void CommandVec::extractParameters(const GateKeeper& aGateKeeper, std::vector<ReadOnlyXParameterSet>& aParamSets, std::vector<std::pair<std::string,std::string> >& aMissingParams, bool throwOnMissing) const
{
  aParamSets.clear();
  aParamSets.reserve( mCommands.size() );

  aMissingParams.clear();

  for( tCommandVector::const_iterator lIt( mCommands.begin()) ; lIt != mCommands.end() ; ++lIt )
  {
    const Command& lCommand( lIt->get() );
    const std::string& lCommandAlias = (lIt->getAlias().empty() ? lCommand.getId() : lIt->getAlias());

    ReadOnlyXParameterSet lParams;
    std::set< std::string > lKeys( lCommand.getDefaultParams().keys() );
    for( std::set< std::string >::iterator lIt2( lKeys.begin() ); lIt2!=lKeys.end(); ++lIt2 )
    {
      GateKeeper::tParameter lData( aGateKeeper.get( getId() , lCommandAlias , *lIt2 , mResource.getGateKeeperTables() ) );
      if ( lData.get() != NULL )
      {
        lParams.adopt( *lIt2 , lData );
      }
      else if (throwOnMissing) {
        std::ostringstream oss;
        oss << "Could not find value of parameter '" << *lIt2 << "' for command with alias '" << lCommandAlias << "' in sequence '" << getId() << "' of resource '" << getResource().getId() << "'";
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




CommandVecStatus::CommandVecStatus(const std::string& aPath, ActionStatus::State aState, float aRunningTime, const Command* aCurrentCommand, const std::vector<CommandStatus>& aStatusOfCompletedCommands,  size_t aTotalNumberOfCommands) : 
  ActionStatus(aPath, aState, aRunningTime),
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


float CommandVecStatus::getProgress() const
{
  if (mCommandStatuses.empty())
    return 0.0;
  else if (mResults.size() == mTotalNumberOfCommands)
    return 1.0;
  else
    return ( float(mResults.size()) + mCommandStatuses.back().getProgress() ) / float(mTotalNumberOfCommands);
}


size_t CommandVecStatus::getNumberOfCompletedCommands() const {
  return mResults.size();
}


size_t CommandVecStatus::getTotalNumberOfCommands() const {
  return mTotalNumberOfCommands;
}


const std::vector<const xdata::Serializable*>& CommandVecStatus::getResults() const
{
  return mResults;
}


const std::vector<CommandStatus>& CommandVecStatus::getCommandStatus() const
{
  return mCommandStatuses;
}


} /* namespace core */
} /* namespace swatch */
