/* 
 * File:   DAQInterface.cpp
 * Author: ale
 * 
 * Created on September 21, 2015, 12:21 PM
 */

#include "swatch/amc13/DAQInterface.hpp"

namespace swatch {
namespace amc13 {


DAQInterface::DAQInterface() :
  core::MonitorableObject( "ttc" ) {
}

DAQInterface::~DAQInterface() {
}


} // namespace amc13
} // namespace swatch


