#include "swatch/core/Command.hpp"


// boost headers
#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"

// XDAQ headers
#include "xdata/Serializable.h"

// SWATCH headers
#include "swatch/core/ActionableObject.hpp"
#include "swatch/core/ThreadPool.hpp"
#include "swatch/logger/Log.hpp"



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
Command::exec(const BusyGuard* aOuterBusyGuard, const XParameterSet& params, bool aUseThreadPool ) 
{
  boost::shared_ptr<BusyGuard> lBusyGuard(new BusyGuard(*this, mActionableStatus, aOuterBusyGuard));

  // Reset the status before doing anything else, merging user-supplied parameter values with default values
  resetForRunning(params);

  // Execute the command protected by a very generic try/catch
  try {
    // if threadpool is to be used
    if ( aUseThreadPool ){
      boost::unique_lock<boost::mutex> lock(mMutex);
      mState = ActionStatus::kScheduled;
      
      ThreadPool& pool = ThreadPool::getInstance();
      pool.addTask<Command,BusyGuard>(this, &Command::runCode, lBusyGuard, mRunningParams);
    }
    else{
      // otherwise execute in same thread
      this->runCode(lBusyGuard, mRunningParams);
    }
  } catch ( const std::exception& e ) {
    // TODO: log the error to error msg (or not?)

    // FIXME: replace with proper logging
    LOG(swatch::logger::kError) << getActionable().getId() << "." << getId() << " : " << ( e.what() != 0x0 ? e.what() : "" ) ;

    // Then rethrow the exception on to the higher layers of code.
    throw;
  }
}


//------------------------------------------------------------------------------------
void Command::runCode(boost::shared_ptr<BusyGuard> aActionGuard, const XParameterSet& params) {
  // 1) Declare that I'm running
  {
    boost::unique_lock<boost::mutex> lock(mMutex);
    mState = ActionStatus::kRunning;
    gettimeofday(&mExecStartTime, NULL);
  }
  
  // 2) Run the code, handling any exceptions
  try {
    State s = this->code(params);
    
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
        // FIXME: replace with proper logging
        LOG(swatch::logger::kError) << getActionable().getId() << "." << getId() << " : " << mStatusMsg;
    }
  }
  catch (const std::exception& e) {
    boost::unique_lock<boost::mutex> lock(mMutex);
    gettimeofday(&mExecEndTime, NULL);
    mState = State::kError;
    mStatusMsg = std::string("An exception of type '" + demangleName(typeid(e).name()) + "' was thrown in Command::code(): ") + e.what();
    // FIXME: replace with proper logging
    LOG(swatch::logger::kError) << getActionable().getId() << "." << getId() << " : " << mStatusMsg;
  }
  
  // 3) Release control of the resource - by destruction of the ActionGuard.
}


//------------------------------------------------------------------------------------
void Command::resetForRunning(const XParameterSet& params) {
  boost::unique_lock<boost::mutex> lock(mMutex);
  mState = ActionStatus::kScheduled;
  mProgress = 0.0;
  mStatusMsg = "";
  
  mResult.reset(mResultCloner(mDefaultResult));
  
  mRunningParams = mergeParametersWithDefaults(params);
}


//------------------------------------------------------------------------------------
ActionStatus::State
Command::getState() const {
  boost::unique_lock<boost::mutex> lock(mMutex);
  return mState;
}


//------------------------------------------------------------------------------------
CommandStatus Command::getStatus() const {
  boost::unique_lock<boost::mutex> lock(mMutex);
  
  // Only let user see the result once the command has completed (since its contents can change before then) ...
  boost::shared_ptr<xdata::Serializable> result((xdata::Serializable*) NULL);
  if ( (mState == ActionStatus::kDone) || (mState == ActionStatus::kWarning) || (mState == ActionStatus::kError))
    result = mResult;
  
  float runningTime = 0.0;
  switch (mState) {
    case ActionStatus::kInitial :
    case ActionStatus::kScheduled : 
      break;
    default:
      timeval endTime;
      if (mState == ActionStatus::kRunning)
        gettimeofday(&endTime, NULL);
      else
        endTime = mExecEndTime;

      runningTime = double(endTime.tv_sec - mExecStartTime.tv_sec);
      runningTime += float(double(endTime.tv_usec) - double(mExecStartTime.tv_usec))/1e6;
      break;
  }
    
  return CommandStatus(getPath(), mState, runningTime, mProgress, mStatusMsg, mRunningParams, result);
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

  // FIXME: replace with proper logging
  LOG(swatch::logger::kInfo) << getActionable().getId() << "." << getId() << " : " << aMsg;

}


void Command::setResult( const xdata::Serializable& aResult ){
  boost::unique_lock<boost::mutex> lock(mMutex);
  mResult->setValue(aResult);
}


//------------------------------------------------------------------------------------
void Command::setStatusMsg(const std::string& aMsg) {
  boost::unique_lock<boost::mutex> lock(mMutex);
  mStatusMsg = aMsg;

  // FIXME: replace with proper logging
  LOG(swatch::logger::kInfo) << getActionable().getId() << "." << getId() << " : " << aMsg;
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
ReadOnlyXParameterSet Command::mergeParametersWithDefaults( const XParameterSet& params ) const {
  ReadOnlyXParameterSet merged = ReadOnlyXParameterSet(params);
  
  std::set<std::string> keys = mDefaultParams.keys();
  BOOST_FOREACH(const std::string& k, keys)
  {
    if ( ! merged.has(k))
      merged.adopt(k, mDefaultParams);
  }
  
  return merged;
}

    
//------------------------------------------------------------------------------------
CommandStatus::CommandStatus(const std::string& aPath, ActionStatus::State aState, float aRunningTime, float aProgress, const std::string& aStatusMsg, const ReadOnlyXParameterSet& aParams, const boost::shared_ptr<xdata::Serializable>& aResult) :
  ActionStatus(aPath, aState, aRunningTime),
  progress_(aProgress),
  statusMsg_(aStatusMsg),
  params_(aParams),
  result_(aResult)
{
}


//------------------------------------------------------------------------------------
float
CommandStatus::getProgress() const {
  return progress_;
}


//------------------------------------------------------------------------------------
const
std::string& CommandStatus::getStatusMsg() const {
  return statusMsg_;
}


//------------------------------------------------------------------------------------
const XParameterSet& CommandStatus::getParameters() const {
  return params_;
}

    
//------------------------------------------------------------------------------------
const xdata::Serializable*
const CommandStatus::getResult() const {
  return result_.get();
}


//------------------------------------------------------------------------------------
std::string
CommandStatus::getResultAsString() const {
  return result_->toString();
}


} // namespace core
} // namespace swatch
