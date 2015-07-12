#include "swatch/core/Command.hpp"


// boost headers
#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"

// XDAQ headers
#include "xdata/Serializable.h"

// SWATCH headers
#include "swatch/core/ThreadPool.hpp"


namespace swatch {
namespace core {

std::ostream& operator<<(std::ostream& out, swatch::core::Command::State s) {
  switch (s) {
    case swatch::core::Command::kInitial   : out << "Initial"; break;
    case swatch::core::Command::kScheduled : out << "Scheduled"; break;
    case swatch::core::Command::kRunning   : out << "Running"; break;
    case swatch::core::Command::kWarning   : out << "Warning"; break;
    case swatch::core::Command::kError     : out << "Error"; break;
    case swatch::core::Command::kDone      : out << "Done"; break;
    default : out << "Unknown value of swatch::core::Command::Status enum"; 
  }
  return out;
}


//---
Command::~Command() {
  if ( defaultResult_ ) delete defaultResult_;
}


//---
void 
Command::exec( const XParameterSet& params  , const bool& aUseThreadPool ) 
{ 
  // Reset the status before doing anything
  reset();

  // merge with default settings
  runningParams_ = mergeParametersWithDefaults(params);

  // Execute the command protected by a very generic try/catch
  try {
    // if threadpool is to be used
    if ( aUseThreadPool ){
      boost::unique_lock<boost::mutex> lock(mutex_);
      state_ = kScheduled;
      
      ThreadPool& pool = ThreadPool::getInstance();
      pool.addTask<Command>(this, &Command::runCode, runningParams_);
    }
    else{
      // otherwise execute in same thread
      this->runCode(runningParams_);
    }
  } catch ( const std::exception& e ) {
    // TODO: log the error to error msg (or not?)
    // Then rethrow the exception on to the higher layers of code.
    throw;
  }
}

void Command::runCode(const XParameterSet& params) {
  // 1) Declare that I'm running
  {
    boost::unique_lock<boost::mutex> lock(mutex_);
    state_ = kRunning;
    gettimeofday(&execStartTime_, NULL);
  }
  
  // 2) Run the code, handling any exceptions
  try {
    State s = this->code(params);
    
    boost::unique_lock<boost::mutex> lock(mutex_);
    gettimeofday(&execEndTime_, NULL);
    switch (s) { 
      case kDone :
      case kWarning :
        progress_ = 1.0;
      case kError : 
        state_ = s;
        break;
      default : 
        state_ = kError;
        statusMsg_ = "ERROR - Command::code() method returned invalid Status enum value '" + boost::lexical_cast<std::string>(s) + "'   \n Original status message was: " + statusMsg_;
    }
  }
  catch (const std::exception& e) {
    boost::unique_lock<boost::mutex> lock(mutex_);
    gettimeofday(&execEndTime_, NULL);
    state_ = kError;
    statusMsg_ = std::string("ERROR - An exception of type '" + demangleName(typeid(e).name()) + "' was thrown in Command::code(): ") + e.what();
  }
}

//---
void Command::reset() {
  boost::unique_lock<boost::mutex> lock(mutex_);
  state_ = kInitial;
  progress_ = 0.0;
  statusMsg_ = "";
  
  result_.reset(resultCloner_(defaultResult_));
}


//---
Command::State
Command::getState() const {
  boost::unique_lock<boost::mutex> lock(mutex_);
  return state_;
}


//---
CommandStatus Command::getStatus() const {
  boost::unique_lock<boost::mutex> lock(mutex_);
  
  // Only let user see the result once the command has completed (since its contents can change before then) ...
  boost::shared_ptr<xdata::Serializable> result((xdata::Serializable*) NULL);
  if ( (state_ == kDone) || (state_ == kWarning) || (state_ == kError))
    result = result_;
  
  float runningTime = 0.0;
  timeval endTime;
  switch (state_) {
    case kInitial :
    case kScheduled : 
      break;
    default:
      if (state_ == kRunning)
        gettimeofday(&endTime, NULL);
      else
        endTime = execEndTime_;

      runningTime = double(endTime.tv_sec - execStartTime_.tv_sec);
      runningTime += float(double(endTime.tv_usec) - double(execStartTime_.tv_usec))/1e6;
      break;
  }
    
  return CommandStatus(state_, runningTime, progress_, statusMsg_, result);
}


//---
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


//---
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
}


void Command::setResult( const xdata::Serializable& aResult ){
  boost::unique_lock<boost::mutex> lock(mutex_);
  result_->setValue(aResult);
}

//---
void Command::setStatusMsg(const std::string& aMsg) {
  boost::unique_lock<boost::mutex> lock(mutex_);
  statusMsg_ = aMsg;
}


xdata::Serializable& Command::defaultResult(){
  return *defaultResult_;
}


const ReadWriteXParameterSet& Command::getDefaultParams() const {
  return defaultParams_;
}


const xdata::Serializable& Command::getDefaultResult() const {
  return *defaultResult_;
}


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




Command::State CommandStatus::getState() const {
  return state_;
}
    

float CommandStatus::getRunningTime() const {
  return runningTime_;
}


float CommandStatus::getProgress() const {
  return progress_;
}


const std::string& CommandStatus::getStatusMsg() const {
  return statusMsg_;
}


const xdata::Serializable* const CommandStatus::getResult() const {
  return result_.get();
}


std::string CommandStatus::getResultAsString() const {
  return result_->toString();
}
    

CommandStatus::CommandStatus(Command::State aState, float aRunningTime, float aProgress, const std::string& aStatusMsg, const boost::shared_ptr<xdata::Serializable>& aResult) :
  state_(aState),
  runningTime_(aRunningTime),
  progress_(aProgress),
  statusMsg_(aStatusMsg),
  result_(aResult)
{
}


} // namespace core
} // namespace swatch
