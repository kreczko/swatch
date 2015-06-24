/**
 * @file    AMC13Commands.cpp
 * @author  Alessandro Thea
 * @date    February 2015
 *
 */

#include "swatch/amc13/AMC13Commands.hpp"


// XDAQ Headers
#include "xdata/Integer.h"
#include "xdata/String.h"

// Swatch Headers
#include "swatch/amc13/AMC13Manager.hpp"
#include "swatch/logger/Log.hpp"

// AMC13 Headers
#include "amc13/AMC13.hh"


namespace swlo = swatch::logger;

namespace swatch {
namespace amc13 {

//---
AMC13ResetCommand::AMC13ResetCommand(const std::string& aId) :
  Command(aId, xdata::Integer()) {
 
  registerParameter("mode", xdata::String());
}


//---
AMC13ResetCommand::~AMC13ResetCommand() {

}


//---
void AMC13ResetCommand::code(core::XParameterSet& params) {

    AMC13Manager* amc13 = getParent<AMC13Manager>();
   
    std::string mode = params.get<xdata::String>("mode");

    using ::amc13::AMC13Simple;
  
    if ( mode == "ttsloopback" ) {
        amc13->driver().reset(AMC13Simple::T1);
        amc13->driver().reset(AMC13Simple::T2);
        LOG(swlo::kInfo) << "Enabling local TTC";
        amc13->driver().localTtcSignalEnable(true);
        
    } else if ( mode == "external" ) {
        amc13->driver().reset(AMC13Simple::T1);
        amc13->driver().reset(AMC13Simple::T2);
        LOG(swlo::kInfo) << "Enabling external TTC input" << std::endl;
        amc13->driver().localTtcSignalEnable(false);
    } else {
      std::ostringstream oss;
        oss << "Unknown option " << mode;
        LOG(swlo::kError) << oss.str();
        setError(oss.str());
        return;
    }
    
    // and check the status
    // amc13->driver()->getStatus()->Report(1);
}


} // namespace amc13
} // namespace swatch
