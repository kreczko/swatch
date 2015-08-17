#include "swatch/core/CommandSequence.hpp"


#include <iostream>

#include "swatch/core/ActionableObject.hpp"
#include "swatch/core/Command.hpp"
#include "swatch/core/GateKeeper.hpp"
#include "swatch/core/XParameterSet.hpp"
#include "swatch/core/ThreadPool.hpp"



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
  
  
CommandSequence::CommandSequence( const std::string& aId ) :
  Functionoid( aId ),
  mTables( NULL ),
  mCommands(),
  mCachedParameters(),
  mParamUpdateTime() ,
  mState( kDone ),
  mCommandIt( mCommands.end() )
{  
}


CommandSequence::~CommandSequence() {
  if( mTables ) delete mTables;
}


CommandSequence& CommandSequence::run( Command& aCommand )
{
  mCommands.push_back( & aCommand );
  mCommandIt = mCommands.end();
  return *this;
}


CommandSequence& CommandSequence::then ( Command& aCommand )
{
  return run( aCommand );
}


CommandSequence& CommandSequence::operator() ( Command& aCommand )
{
  return run( aCommand );
}


CommandSequence& CommandSequence::run( const std::string& aCommand )
{
  ActionableObject* lParent( getParent<ActionableObject>()  );
  return run( lParent->getCommand( aCommand ) );
}


CommandSequence& CommandSequence::then ( const std::string& aCommand )
{
  return run( aCommand );
}


CommandSequence& CommandSequence::operator() ( const std::string& aCommand )
{
  return run( aCommand );
}


const std::vector<std::string>& CommandSequence::getTables()
{
  if( !mTables ) mTables = setTables();
  return *mTables;
}


const std::vector<Command*> CommandSequence::getCommands() const 
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




void CommandSequence::exec(GateKeeper& aGateKeeper, const bool& aUseThreadPool )
{
  // 1) TODO: Request sole control of the resource - then must change later code from calling "Command::exec" to calling "Command::runCode" / similar
  
  
  // 2) Reset the status of this sequence's state variables
  {
    boost::unique_lock<boost::mutex> lock( mMutex );
    mState = kInitial;
    mCommandIt = mCommands.end();
    mStatusOfCompletedCommands.clear();
    mStatusOfCompletedCommands.reserve(mCommands.size());
  }  

  
  // 3) Extract parameters from gatekeeper
  // Is our cache of parameters up to date?
  boost::posix_time::ptime lUpdateTime( aGateKeeper.lastUpdated() );
  if( mParamUpdateTime != lUpdateTime )
  {
    updateParameterCache(aGateKeeper);
    mParamUpdateTime = lUpdateTime; // We are up to date :)
  }

  // 3) Execute the command
  if ( aUseThreadPool){
    boost::unique_lock<boost::mutex> lock(mMutex);
    mState = kScheduled;
    
    ThreadPool& pool = ThreadPool::getInstance();
    pool.addTask<CommandSequence>( this , &CommandSequence::runCommands );
  }
  else{
    // otherwise execute in same thread
    this->runCommands();
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
  
  const Command* currentCommand =  ( (mCommandIt == mCommands.end()) ? NULL : *mCommandIt);
  
  return CommandSequenceStatus(mState, runningTime, currentCommand, mStatusOfCompletedCommands, mCommands.size());
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


void CommandSequence::runCommands()
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
      (*mCommandIt)->exec( *lIt , false ); // False = run the commands in this thread!
      //FIXME: Make exec method return CommandStatus to remove any possibility of race condition ?
      CommandStatus status = (*mCommandIt)->getStatus();
      
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
  
  // TODO: 3) Release the resource
}


void CommandSequence::updateParameterCache(GateKeeper& aGateKeeper)
{
  mCachedParameters.clear();
  mCachedParameters.reserve( mCommands.size() );

  for( tCommandVector::iterator lIt( mCommands.begin()) ; lIt != mCommands.end() ; ++lIt )
  {
    Command& lCommand( **lIt );
    ReadOnlyXParameterSet lParams( lCommand.getDefaultParams() );

    std::set< std::string > lKeys( lParams.keys() );
    for( std::set< std::string >::iterator lIt2( lKeys.begin() ); lIt2!=lKeys.end(); ++lIt2 )
    {
      GateKeeper::tParameter lData( aGateKeeper.get( getPath() , lCommand.getId() , *lIt2 , getTables() ) );
      if ( lData.get() != NULL )
      {
        lParams.erase(*lIt2);
        lParams.adopt( *lIt2 , lData );
      }
    }
    mCachedParameters.push_back( lParams );
  }
}




CommandSequenceStatus::CommandSequenceStatus(CommandSequence::State aState, float aRunningTime, const Command* aCurrentCommand, const std::vector<CommandStatus>& aStatusOfCompletedCommands, const size_t& aTotalNumberOfCommands) : 
  mState(aState),
  mRunningTime(aRunningTime),
  mCurrentCommand(aCurrentCommand),
  mProgress( float(aStatusOfCompletedCommands.size()) / float(aTotalNumberOfCommands) ),
  mCommandStatuses(aStatusOfCompletedCommands)
{
  if (mCurrentCommand != NULL)
  {
    mCommandStatuses.push_back(mCurrentCommand->getStatus());
    mProgress += ( mCommandStatuses.back().getProgress() / float(aTotalNumberOfCommands) );
  } 
 
  for(auto it = aStatusOfCompletedCommands.begin(); it != aStatusOfCompletedCommands.end(); it++)
    mResults.push_back(it->getResult());
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
  return mProgress;
}


const Command* CommandSequenceStatus::getCurrentCommand() const
{
  return mCurrentCommand;
}


size_t CommandSequenceStatus::getNumberOfCompletedCommands() const
{
  return mResults.size();
}


const std::vector<const xdata::Serializable*>& CommandSequenceStatus::getResults() const
{
  return mResults;
}


const std::vector<CommandStatus>& CommandSequenceStatus::getCommandStatus() const
{
  return mCommandStatuses;
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






// std::string CommandSequence::mCommandSequenceComplete = std::string( "CommandSequence complete" );

} /* namespace core */
} /* namespace swatch */
