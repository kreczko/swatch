#include "swatch/core/Command.hpp"

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
Command::exec() {
  // Reset the status before doing anything
  reset();

  // Execute the command protected by a very generic try/catch
  try {
    this->code();
  } catch ( const std::exception& e ) {
    // TODO: log the error to error msg (or not?)
    // Then rethrow the exception on to the higher layers of code.
    throw;
  }
}

//---
XParameterSet&
Command::getParams() {
  return parameters_;
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
  status_ = kDone;
  statusMsg_ = aMsg;
}


//---
void
Command::setWarning(const std::string& aMsg) {
  status_ = kWarning;
  statusMsg_ = aMsg;
}


//---
void
Command::setError(const std::string& aMsg) {
  status_ = kError;
  statusMsg_ = aMsg;
}


//---
const std::string& Command::getProgressMsg() const {
  return progressMsg_;
}


//---
void
Command::setProgress(float aProgress) {
  progress_ = aProgress;
}

//---
void
Command::setProgress(float aProgress, const std::string& aMsg ) {
  if ( aProgress < 0. or aProgress > 100.) {
    std::ostringstream err;
    err << "Progress must be in the [0.,100.] range. " << aProgress;
    throw std::out_of_range(err.str());
  }
  
  progress_ = aProgress;
  progressMsg_ = aMsg;
}

//---
void Command::setStatusMsg(const std::string& aMsg) {
  statusMsg_ = aMsg;
}

void Command::setResult( const xdata::Serializable& aResult ){
  result_->setValue(aResult);
}

xdata::Serializable& Command::defaultResult(){
  return *default_;
}

const std::string& Command::getStatusMsg() const {
  return statusMsg_;
}

void Command::setStatus( Status aStatus ) {
  status_ = aStatus;
}

} // namespace core
} // namespace swatch
