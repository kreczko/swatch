#include "swatch/core/Command.hpp"


// boost headers
#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"

// XDAQ headers
#include "xdata/Serializable.h"

// SWATCH headers
#include "swatch/core/ActionableObject.hpp"
#include "swatch/core/ThreadPool.hpp"

// log4cplus headers
#include <log4cplus/loggingmacros.h>



namespace swatch {
namespace core {


//------------------------------------------------------------------------------------
Command::~Command() {
  if ( mDefaultResult ) delete mDefaultResult;
}


//------------------------------------------------------------------------------------
void 
Command::exec( const XParameterSet& aParams, bool aUseThreadPool ) 
{
  exec(NULL, aParams, aUseThreadPool);
}


//------------------------------------------------------------------------------------
void 
Command::exec(const BusyGuard* aOuterBusyGuard, const XParameterSet& aParams, bool aUseThreadPool ) 
{
  boost::shared_ptr<BusyGuard> lBusyGuard(new BusyGuard(*this, mActionableStatus, aOuterBusyGuard));

  // Reset the status before doing anything else, merging user-supplied parameter values with default values
  resetForRunning(aParams);

  // Execute the command protected by a very generic try/catch
  try {
    // if threadpool is to be used
    if ( aUseThreadPool ){
      boost::unique_lock<boost::mutex> lock(mMutex);
      mState = ActionSnapshot::kScheduled;
      
      ThreadPool& pool = ThreadPool::getInstance();
      pool.addTask<Command,BusyGuard>(this, &Command::runCode, lBusyGuard, mRunningParams);
    }
    else{
      // otherwise execute in same thread
      this->runCode(lBusyGuard, mRunningParams);
    }
  } catch ( const std::exception& e ) {
    LOG4CPLUS_ERROR(getActionable().getLogger(), "Exception thrown when executing command '" << getId() << "' : " << ( e.what() != 0x0 ? e.what() : "" ));

    // Then rethrow the exception on to the higher layers of code.
    throw;
  }
}


//------------------------------------------------------------------------------------
void Command::runCode(boost::shared_ptr<BusyGuard> aActionGuard, const XParameterSet& aParams) {
  // 1) Declare that I'm running
  {
    boost::unique_lock<boost::mutex> lock(mMutex);
    mState = ActionSnapshot::kRunning;
    gettimeofday(&mExecStartTime, NULL);
  }
  
  // 2) Run the code, handling any exceptions
  try {
    State s = this->code(aParams);
    
    boost::unique_lock<boost::mutex> lock(mMutex);
    gettimeofday(&mExecEndTime, NULL);
    switch (s) { 
      case State::kDone :
      case State::kWarning :
        mProgress = 1.0;
      case State::kError : 
        mState = s;
        break;
      default : 
        mState = State::kError;
        mStatusMsg = "Command::code() method returned invalid Status enum value '" + boost::lexical_cast<std::string>(s) + "'   \n Original status message was: " + mStatusMsg;
        LOG4CPLUS_ERROR(getActionable().getLogger(), "Command '" << getId() << " : " << mStatusMsg);
    }
  }
  catch (const std::exception& e) {
    boost::unique_lock<boost::mutex> lock(mMutex);
    gettimeofday(&mExecEndTime, NULL);
    mState = State::kError;
    mStatusMsg = std::string("An exception of type '" + demangleName(typeid(e).name()) + "' was thrown in Command::code(): ") + e.what();
    LOG4CPLUS_ERROR(getActionable().getLogger(), "Command '" << getId() << " : " << mStatusMsg);
  }
  
  // 3) Release control of the resource - by destruction of the ActionGuard.
}


//------------------------------------------------------------------------------------
void Command::resetForRunning(const XParameterSet& aParams) {
  boost::unique_lock<boost::mutex> lock(mMutex);
  mState = ActionSnapshot::kScheduled;
  mProgress = 0.0;
  mStatusMsg = "";
  
  mResult.reset(mResultCloner(mDefaultResult));
  
  mRunningParams = mergeParametersWithDefaults(aParams);
}


//------------------------------------------------------------------------------------
ActionSnapshot::State
Command::getState() const {
  boost::unique_lock<boost::mutex> lock(mMutex);
  return mState;
}


//------------------------------------------------------------------------------------
CommandSnapshot Command::getStatus() const {
  boost::unique_lock<boost::mutex> lock(mMutex);
  
  // Only let user see the result once the command has completed (since its contents can change before then) ...
  boost::shared_ptr<xdata::Serializable> result((xdata::Serializable*) NULL);
  if ( (mState == ActionSnapshot::kDone) || (mState == ActionSnapshot::kWarning) || (mState == ActionSnapshot::kError))
    result = mResult;
  
  float runningTime = 0.0;
  switch (mState) {
    case ActionSnapshot::kInitial :
    case ActionSnapshot::kScheduled : 
      break;
    default:
      timeval endTime;
      if (mState == ActionSnapshot::kRunning)
        gettimeofday(&endTime, NULL);
      else
        endTime = mExecEndTime;

      runningTime = double(endTime.tv_sec - mExecStartTime.tv_sec);
      runningTime += float(double(endTime.tv_usec) - double(mExecStartTime.tv_usec))/1e6;
      break;
  }
    
  return CommandSnapshot(getPath(), mState, runningTime, mProgress, mStatusMsg, mRunningParams, result);
}


//------------------------------------------------------------------------------------
void
Command::setProgress(float aProgress) {
  if ( aProgress < 0. or aProgress > 1.) {
    std::ostringstream err;
    err << "Progress must be in the [0.,1.] range. " << aProgress;
    // TODO: should this not throw a SWATCH exception?
    throw std::out_of_range(err.str());
  }

  boost::unique_lock<boost::mutex> lock(mMutex);
  mProgress = aProgress;
}


//------------------------------------------------------------------------------------
void
Command::setProgress(float aProgress, const std::string& aMsg ) {
  if ( aProgress < 0. or aProgress > 1.) {
    std::ostringstream err;
    err << "Progress must be in the [0.,1.] range. " << aProgress;
    // TODO: should this not throw a SWATCH exception?
    throw std::out_of_range(err.str());
  }

  boost::unique_lock<boost::mutex> lock(mMutex);
  mProgress = aProgress;
  mStatusMsg = aMsg;

  LOG4CPLUS_DEBUG(getActionable().getLogger(), "Command '" << getId() << "' : " << aMsg);
}


//------------------------------------------------------------------------------------
void Command::setResult( const xdata::Serializable& aResult ){
  boost::unique_lock<boost::mutex> lock(mMutex);
  mResult->setValue(aResult);
}


//------------------------------------------------------------------------------------
void Command::setStatusMsg(const std::string& aMsg) {
  boost::unique_lock<boost::mutex> lock(mMutex);
  mStatusMsg = aMsg;

  LOG4CPLUS_DEBUG(getActionable().getLogger(), "Command '" << getId() << "' : " << aMsg);
}


//------------------------------------------------------------------------------------
void Command::unregisterParameter(const std::string name)
{
  if ( getPath() != getId() ) {
    // The Command has already been registered. Parameters list cannot be modified
    throw CommandParameterRegistrationFailed("Registering parameter outside constructors is not allowed");
  }
    
  mDefaultParams.erase(name);
}

//------------------------------------------------------------------------------------
const ReadWriteXParameterSet& Command::getDefaultParams() const {
  return mDefaultParams;
}


//------------------------------------------------------------------------------------
const xdata::Serializable& Command::getDefaultResult() const {
  return *mDefaultResult;
}


//------------------------------------------------------------------------------------
ReadOnlyXParameterSet Command::mergeParametersWithDefaults( const XParameterSet& aParams ) const {
  ReadOnlyXParameterSet merged = ReadOnlyXParameterSet(aParams);
  
  std::set<std::string> keys = mDefaultParams.keys();
  BOOST_FOREACH(const std::string& k, keys)
  {
    if ( ! merged.has(k))
      merged.adopt(k, mDefaultParams);
  }
  
  return merged;
}

    
//------------------------------------------------------------------------------------
CommandSnapshot::CommandSnapshot(const std::string& aPath, ActionSnapshot::State aState, float aRunningTime, float aProgress, const std::string& aStatusMsg, const ReadOnlyXParameterSet& aParams, const boost::shared_ptr<xdata::Serializable>& aResult) :
  ActionSnapshot(aPath, aState, aRunningTime),
  mProgress(aProgress),
  mStatusMsg(aStatusMsg),
  mParams(aParams),
  mResult(aResult)
{
}


//------------------------------------------------------------------------------------
float
CommandSnapshot::getProgress() const {
  return mProgress;
}


//------------------------------------------------------------------------------------
const
std::string& CommandSnapshot::getStatusMsg() const {
  return mStatusMsg;
}


//------------------------------------------------------------------------------------
const XParameterSet& CommandSnapshot::getParameters() const {
  return mParams;
}

    
//------------------------------------------------------------------------------------
const xdata::Serializable*
const CommandSnapshot::getResult() const {
  return mResult.get();
}


//------------------------------------------------------------------------------------
std::string
CommandSnapshot::getResultAsString() const {
  return mResult->toString();
}


} // namespace core
} // namespace swatch
