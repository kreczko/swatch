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


CommandVec::CommandVec( const std::string& aId, ActionableObject& aActionable) :
  ObjectFunctionoid( aId, aActionable ),
  mCommands(),
  mCachedParameters(),
  mParamUpdateTime() ,
  mState( ActionStatus::kInitial ),
  mCommandIt( mCommands.end() )
{
}


void CommandVec::addCommand(Command& aCommand, const std::string& aNamespace)
{
  if( &aCommand.getActionable() != &getActionable() )
  {
    throw InvalidResource("Cannot add command '" + aCommand.getId() + "' (resource: "+aCommand.getActionable().getPath()+") ' to sequence of resource '" + getActionable().getPath() + "'");
  }
  mCommands.push_back( Element(aCommand, aNamespace) );
  mCommandIt = mCommands.end();
}


void CommandVec::addCommand(const std::string& aCommand, const std::string& aNamespace)
{
  Command& lCommand = getActionable().getCommand( aCommand );
  mCommands.push_back( Element(lCommand, aNamespace) );
  mCommandIt = mCommands.end();
}


CommandVec::~CommandVec() {
}


CommandVec::Element::Element(Command& aCommand, const std::string& aNamespace) :
  mCmd(&aCommand),
  mNamespace(aNamespace)
{
}


CommandVec::Element::~Element()
{
}

const std::string& CommandVec::Element::getNamespace() const
{
  return mNamespace;
}

const Command& CommandVec::Element::get() const
{
  return *mCmd;
}

Command& CommandVec::Element::get()
{
  return *mCmd;
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
    lCmds.push_back(&lIt->get());
  return lCmds;
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
  
  const Command* currentCommand =  ( ((mCommandIt == mCommands.end()) || (mState == State::kError)) ? NULL : &mCommandIt->get());
  
  return CommandVecStatus(getPath(), mState, runningTime, currentCommand, mStatusOfCompletedCommands, mCommands.size());
}


void CommandVec::runCommands(boost::shared_ptr<BusyGuard> aGuard)
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
    ParameterSets_t::iterator lIt( mCachedParameters.begin() );
    
    while( true ) 
    {
      mCommandIt->get().exec(aGuard.get(), *lIt , false ); // False = run the commands in this thread!
      //FIXME: Make exec method return CommandStatus to remove any possibility of race condition ?

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


CommandVec::MissingParam::MissingParam(const std::string& aNamespace, const std::string& aCommand, const std::string& aParam) :
  nspace(aNamespace),
  command(aCommand),
  parameter(aParam)
{
}


void CommandVec::checkForMissingParameters(const GateKeeper& aGateKeeper, std::vector<ReadOnlyXParameterSet>& aParamSets, std::vector<MissingParam>& aMissingParams) const
{
  extractParameters(aGateKeeper, aParamSets, aMissingParams, false);
}


void CommandVec::reset(const ParameterSets_t& aParamSets)
{
  boost::unique_lock<boost::mutex> lock( mMutex );

  mState = ActionStatus::kInitial;
  mCommandIt = mCommands.end();
  mCachedParameters = aParamSets;
  mStatusOfCompletedCommands.clear();
  mStatusOfCompletedCommands.reserve(mCommands.size());
}


void CommandVec::extractParameters(const GateKeeper& aGateKeeper, std::vector<ReadOnlyXParameterSet>& aParamSets, std::vector<MissingParam>& aMissingParams, bool throwOnMissing) const
{
  aParamSets.clear();
  aParamSets.reserve( mCommands.size() );

  aMissingParams.clear();

  for( CommandVector_t::const_iterator lIt( mCommands.begin()) ; lIt != mCommands.end() ; ++lIt )
  {
    const Command& lCommand( lIt->get() );
//    const std::string& lCommandAlias = (lIt->getAlias().empty() ? lCommand.getId() : lIt->getAlias());

    ReadOnlyXParameterSet lParams;
    std::set< std::string > lKeys( lCommand.getDefaultParams().keys() );
    for( std::set< std::string >::iterator lIt2( lKeys.begin() ); lIt2!=lKeys.end(); ++lIt2 )
    {
      GateKeeper::tParameter lData( aGateKeeper.get( lIt->getNamespace() , lCommand.getId() , *lIt2 , getActionable().getGateKeeperTables() ) );
      if ( lData.get() != NULL )
      {
        lParams.adopt( *lIt2 , lData );
      }
      else if (throwOnMissing) {
        std::ostringstream oss;
        oss << "Could not find value of parameter '" << *lIt2 << "' for command '" << lCommand.getId() << "' in namespace '" << lIt->getNamespace() << "' of resource '" << getActionable().getId() << "'";
        LOG(swatch::logger::kError) << oss.str();
        throw ParameterNotFound(oss.str());
      }
      else {
        aMissingParams.push_back(MissingParam(lIt->getNamespace(), lCommand.getId(), *lIt2));
      }
    }
    aParamSets.push_back( lParams );
  }
}

std::ostream& operator << (std::ostream& aOstream, const CommandVec::MissingParam& aMissingParam )
{
  return (aOstream << aMissingParam.nspace << "." << aMissingParam.command << "." << aMissingParam.parameter);
}


bool operator !=(const CommandVec::MissingParam& l1, const CommandVec::MissingParam& l2)
{
  return !( (l1.nspace == l2.nspace) && (l1.command == l2.command) && (l1.parameter == l2.parameter));
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
  if ((mTotalNumberOfCommands == 0) && (getState() == kDone))
    return 1.0;
  else if (mCommandStatuses.empty())
    return 0.0;
  else if ((getState() == kDone) || (getState() == kWarning))
    return 1.0;
  else
    return ( float(mCommandStatuses.size()-1) + mCommandStatuses.back().getProgress() ) / float(mTotalNumberOfCommands);
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
