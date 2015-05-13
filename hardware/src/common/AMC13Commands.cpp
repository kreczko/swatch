/**
 * @file    AMC13Commands.cpp
 * @author  Alessandro Thea
 * @date    February 2015
 *
 */

#include <xdata/String.h>

#include "swatch/hardware/AMC13Commands.hpp"

// XDAQ Headers
#include "xdata/Integer.h"

// Swatch Headers
#include "swatch/hardware/AMC13Manager.hpp"
#include "swatch/logger/Log.hpp"

// AMC13 Headers
#include "amc13/AMC13.hh"


namespace swlo = swatch::logger;

namespace swatch {
namespace hardware {

//---
AMC13ResetCommand::AMC13ResetCommand(core::ActionableObject* aHandler) :
  Command(aHandler, xdata::Integer()) {
  
  getParams().add("mode", xdata::String());

}


//---
AMC13ResetCommand::~AMC13ResetCommand() {

}


//---
void AMC13ResetCommand::code() {
  

    AMC13Manager* amc13 = getParent<AMC13Manager>();
    
  
    std::string mode = getParams().get<xdata::String>("mode");
  
    if ( mode == "ttsloopback" ) {
        amc13->driver()->reset(amc13::AMC13Simple::T1);
        amc13->driver()->reset(amc13::AMC13Simple::T2);
        LOG(swlo::kInfo) << "Enabling local TTC";
        amc13->driver()->localTtcSignalEnable(true);
        
    } else if ( mode == "external" ) {
        amc13->driver()->reset(amc13::AMC13Simple::T1);
        amc13->driver()->reset(amc13::AMC13Simple::T2);
        LOG(swlo::kInfo) << "Enabling external TTC input" << std::endl;
        amc13->driver()->localTtcSignalEnable(false);
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


} // namespace hardware
} // namespace swatch
