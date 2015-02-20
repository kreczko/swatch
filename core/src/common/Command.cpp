#include "swatch/core/Command.hpp"

// XDAQ Headers
#include "xdata/Serializable.h"

namespace swatch {
namespace core {

// Command::Command(CommandManager* aMgr, xdata::Serializable* aResult) : 
//   progress_(0.), 
//   result_(aResult),
//   manager_(aMgr) {
// }

//---
Command::~Command() {
  if ( result_ ) delete result_;
  if ( default_ ) delete default_;
}


//---
XParameterSet&
Command::getParameters() {
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
Command::status() const {
  return status_;
}

//---
float Command::progress() const {
  return progress_;
}


//---
xdata::Serializable& Command::result() {
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
const std::string& Command::progressMsg() const {
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

} // namespace core
} // namespace swatch