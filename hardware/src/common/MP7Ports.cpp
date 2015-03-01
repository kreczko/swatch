/**
 * @file    MP7Ports.cpp
 * @author  Alessandro Thea
 * @brief   MP7Processor Rx and Tx ports implementations
 * @date    February 2015
 */


#include "swatch/hardware/MP7Ports.hpp"

#include "mp7/MP7Controller.hpp"

namespace swatch {
namespace hardware {

//---
MP7RxPort::MP7RxPort( const std::string& aId, mp7::MP7Controller* aDriver ) :
  InputPort(aId),
  driver_(aDriver){
}


//---
MP7RxPort::~MP7RxPort() {
}

bool
MP7RxPort::isEnabled() const {
  return true;
}

bool
MP7RxPort::isOperating() const {
  return true;
}

uint32_t
MP7RxPort::getErrors() const {
  return 0;
}


}
}
