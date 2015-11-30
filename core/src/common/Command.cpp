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
  if ( defaultResult_ ) delete defaultResult_;
}


//------------------------------------------------------------------------------------
void 
Command::exec( const XParameterSet& aParams, bool aUseThreadPool ) 
{
  exec(NULL, aParams, aUseThreadPool);
}


//------------------------------------------------------------------------------------
void 
Command::exec(const ActionableObject::BusyGuard* aOuterBusyGuard, const XParameterSet& params, bool aUseThreadPool ) 
{
  boost::shared_ptr<ActionableObject::BusyGuard> lBusyGuard(new ActionableObject::BusyGuard(*this, mActionableStatus, aOuterBusyGuard));

  // Reset the status before doing anything else, merging user-supplied parameter values with default values
  resetForRunning(params);

  // Execute the command protected by a very generic try/catch
  try {
    // if threadpool is to be used
    if ( aUseThreadPool ){
      boost::unique_lock<boost::mutex> lock(mutex_);
      state_ = ActionStatus::kScheduled;
      
      ThreadPool& pool = ThreadPool::getInstance();
      pool.addTask<Command,ActionableObject::BusyGuard>(this, &Command::runCode, lBusyGuard, runningParams_);
    }
    else{
      // otherwise execute in same thread
      this->runCode(lBusyGuard, runningParams_);
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
void Command::runCode(boost::shared_ptr<ActionableObject::BusyGuard> aActionGuard, const XParameterSet& params) {
  // 1) Declare that I'm running
  {
    boost::unique_lock<boost::mutex> lock(mutex_);
    state_ = ActionStatus::kRunning;
    gettimeofday(&execStartTime_, NULL);
  }
  
  // 2) Run the code, handling any exceptions
  try {
    State s = this->code(params);
    
    boost::unique_lock<boost::mutex> lock(mutex_);
    gettimeofday(&execEndTime_, NULL);
    switch (s) { 
      case State::kDone :
      case State::kWarning :
        progress_ = 1.0;
      case State::kError : 
        state_ = s;
        break;
      default : 
        state_ = State::kError;
        statusMsg_ = "Command::code() method returned invalid Status enum value '" + boost::lexical_cast<std::string>(s) + "'   \n Original status message was: " + statusMsg_;
        // FIXME: replace with proper logging
        LOG(swatch::logger::kError) << getActionable().getId() << "." << getId() << " : " << statusMsg_;
    }
  }
  catch (const std::exception& e) {
    boost::unique_lock<boost::mutex> lock(mutex_);
    gettimeofday(&execEndTime_, NULL);
    state_ = State::kError;
    statusMsg_ = std::string("An exception of type '" + demangleName(typeid(e).name()) + "' was thrown in Command::code(): ") + e.what();
    // FIXME: replace with proper logging
    LOG(swatch::logger::kError) << getActionable().getId() << "." << getId() << " : " << statusMsg_;
  }
  
  // 3) Release control of the resource - by destruction of the ActionGuard.
}


//------------------------------------------------------------------------------------
void Command::resetForRunning(const XParameterSet& params) {
  boost::unique_lock<boost::mutex> lock(mutex_);
  state_ = ActionStatus::kScheduled;
  progress_ = 0.0;
  statusMsg_ = "";
  
  result_.reset(resultCloner_(defaultResult_));
  
  runningParams_ = mergeParametersWithDefaults(params);
}


//------------------------------------------------------------------------------------
ActionStatus::State
Command::getState() const {
  boost::unique_lock<boost::mutex> lock(mutex_);
  return state_;
}


//------------------------------------------------------------------------------------
CommandStatus Command::getStatus() const {
  boost::unique_lock<boost::mutex> lock(mutex_);
  
  // Only let user see the result once the command has completed (since its contents can change before then) ...
  boost::shared_ptr<xdata::Serializable> result((xdata::Serializable*) NULL);
  if ( (state_ == ActionStatus::kDone) || (state_ == ActionStatus::kWarning) || (state_ == ActionStatus::kError))
    result = result_;
  
  float runningTime = 0.0;
  switch (state_) {
    case ActionStatus::kInitial :
    case ActionStatus::kScheduled : 
      break;
    default:
      timeval endTime;
      if (state_ == ActionStatus::kRunning)
        gettimeofday(&endTime, NULL);
      else
        endTime = execEndTime_;

      runningTime = double(endTime.tv_sec - execStartTime_.tv_sec);
      runningTime += float(double(endTime.tv_usec) - double(execStartTime_.tv_usec))/1e6;
      break;
  }
    
  return CommandStatus(getPath(), state_, runningTime, progress_, statusMsg_, runningParams_, result);
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

  boost::unique_lock<boost::mutex> lock(mutex_);
  progress_ = aProgress;
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

  boost::unique_lock<boost::mutex> lock(mutex_);
  progress_ = aProgress;
  statusMsg_ = aMsg;

  // FIXME: replace with proper logging
  LOG(swatch::logger::kInfo) << getActionable().getId() << "." << getId() << " : " << aMsg;

}


void Command::setResult( const xdata::Serializable& aResult ){
  boost::unique_lock<boost::mutex> lock(mutex_);
  result_->setValue(aResult);
}


//------------------------------------------------------------------------------------
void Command::setStatusMsg(const std::string& aMsg) {
  boost::unique_lock<boost::mutex> lock(mutex_);
  statusMsg_ = aMsg;

  // FIXME: replace with proper logging
  LOG(swatch::logger::kInfo) << getActionable().getId() << "." << getId() << " : " << aMsg;
}


//------------------------------------------------------------------------------------
const ReadWriteXParameterSet& Command::getDefaultParams() const {
  return defaultParams_;
}


//------------------------------------------------------------------------------------
const xdata::Serializable& Command::getDefaultResult() const {
  return *defaultResult_;
}


//------------------------------------------------------------------------------------
ReadOnlyXParameterSet Command::mergeParametersWithDefaults( const XParameterSet& params ) const {
  ReadOnlyXParameterSet merged = ReadOnlyXParameterSet(params);
  
  std::set<std::string> keys = defaultParams_.keys();
  BOOST_FOREACH(const std::string& k, keys)
  {
    if ( ! merged.has(k))
      merged.adopt(k, defaultParams_);
  }
  
  return merged;
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

    
//------------------------------------------------------------------------------------
CommandStatus::CommandStatus(const std::string& aPath, ActionStatus::State aState, float aRunningTime, float aProgress, const std::string& aStatusMsg, const ReadOnlyXParameterSet& aParams, const boost::shared_ptr<xdata::Serializable>& aResult) :
  ActionStatus(aPath, aState, aRunningTime),
  progress_(aProgress),
  statusMsg_(aStatusMsg),
  params_(aParams),
  result_(aResult)
{
}


} // namespace core
} // namespace swatch
