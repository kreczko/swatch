/**
 * @file    AMC13Operations.cpp
 * @author  Alessandro Thea
 * @date    March 2015
 *
 */


#include "swatch/hardware/AMC13Operations.hpp"

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
AMC13Configure::AMC13Configure(core::ActionableObject* aHandler) :
  swatch::core::Operation(aHandler) {
  
}


//---
AMC13Configure::~AMC13Configure() {

}


} // namespace hardware
} // namespace swatch
