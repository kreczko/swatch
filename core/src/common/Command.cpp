#include "swatch/core/Command.hpp"
#include "swatch/core/ThreadPool.hpp"


// XDAQ Headers
#include "xdata/Serializable.h"

namespace swatch {
namespace core {

//---
Command::~Command() {
  if ( result_ ) delete result_;
  if ( default_ ) delete default_;
}


//---
void 
Command::exec( XParameterSet& params  , const bool& aUseThreadPool ) ///Should take const reference but xdata::serializable is const-correctness broken
{ 
  // Reset the status before doing anything
  reset();

  // merge with default settings
  XParameterSet p = mergeParametersWithDefaults(params);

  // Execute the command protected by a very generic try/catch
  try {
    // if threadpool is to be used
    if ( aUseThreadPool ){
      ThreadPool& pool = ThreadPool::getInstance();
      pool.addTask<Command>(this, &Command::runCode, p);
    }
    else{
      // otherwise execute in same thread
      this->runCode(p);
    }
  } catch ( const std::exception& e ) {
    // TODO: log the error to error msg (or not?)
    // Then rethrow the exception on to the higher layers of code.
    throw;
  }
}

void Command::runCode(XParameterSet& params) {
  try {
    this->code(params);
  } catch (const std::exception& e) {
    this->setError( "An exception occured in Command::code(): " + std::string(e.what()));
  }
}

//---
void Command::reset() {
    setStatus(kInitial),
    setResult(defaultResult());
    setProgress(0);
    setStatusMsg("");
}

//---
Command::Status
Command::getStatus() const {
  return status_;
}

//---
float Command::getProgress() const {
  return progress_;
}


//---
xdata::Serializable& Command::getResult() {
  return *result_;
}


//---
void
Command::setDone(const std::string& aMsg) {
  setProgress(100.);
  {
    boost::unique_lock<boost::mutex> lock(status_mutex_);
    status_ = kDone;
    statusMsg_ = aMsg;
  }
}


//---
void
Command::setWarning(const std::string& aMsg) {
  {
    boost::unique_lock<boost::mutex> lock(status_mutex_);
    status_ = kWarning;
    statusMsg_ = aMsg;
  }
}


//---
void
Command::setError(const std::string& aMsg) {
  {
    boost::unique_lock<boost::mutex> lock(status_mutex_);
    status_ = kError;
    statusMsg_ = aMsg;
  }
}


//---
std::string Command::getProgressMsg() {
  boost::unique_lock<boost::mutex> lock(progress_mutex_);
  return progressMsg_;
}


//---
void
Command::setProgress(float aProgress) {
  {
    boost::unique_lock<boost::mutex> lock(progress_mutex_);
    progress_ = aProgress;
  }
}

//---
void
Command::setProgress(float aProgress, const std::string& aMsg ) {
  if ( aProgress < 0. or aProgress > 100.) {
    std::ostringstream err;
    err << "Progress must be in the [0.,100.] range. " << aProgress;
    // TODO: should this not throw a SWATCH exception?
    throw std::out_of_range(err.str());
  }
  {
    boost::unique_lock<boost::mutex> lock(progress_mutex_);
    progress_ = aProgress;
    progressMsg_ = aMsg;
  }
}

//---
void Command::setStatusMsg(const std::string& aMsg) {
  {
    boost::unique_lock<boost::mutex> lock(status_mutex_);
    statusMsg_ = aMsg;
  }
}

void Command::setResult( const xdata::Serializable& aResult ){
  {
    boost::unique_lock<boost::mutex> lock(result_mutex_);
    result_->setValue(aResult);
  }
}

xdata::Serializable& Command::defaultResult(){
  return *default_;
}

std::string Command::getStatusMsg() {
  boost::unique_lock<boost::mutex> lock(status_mutex_);
  return statusMsg_;
}

void Command::setStatus( Status aStatus ) {
  {
    boost::unique_lock<boost::mutex> lock(status_mutex_);
    status_ = aStatus;
  }
}

const XParameterSet& Command::getDefaultParams() const {
  return parameters_;
}

XParameterSet Command::mergeParametersWithDefaults( XParameterSet& params ) const {
  XParameterSet merged = XParameterSet(params);
  merged.update(parameters_);
  return merged;
}

} // namespace core
} // namespace swatch
