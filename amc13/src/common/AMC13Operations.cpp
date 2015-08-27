/**
 * @file    AMC13Operations.cpp
 * @author  Alessandro Thea
 * @date    March 2015
 *
 */

#include "swatch/amc13/AMC13Operations.hpp"


// XDAQ Headers
#include "xdata/Integer.h"

// Swatch Headers
#include "swatch/amc13/AMC13Manager.hpp"
#include "swatch/logger/Log.hpp"

// AMC13 Headers
#include "amc13/AMC13.hh"


namespace swlo = swatch::logger;

namespace swatch {
namespace amc13 {

//---
AMC13Configure::AMC13Configure(const std::string& aId) :
  swatch::core::Operation(aId) {
  
}


//---
AMC13Configure::~AMC13Configure() {

}


} // namespace amc13
} // namespace swatch