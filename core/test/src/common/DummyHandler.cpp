/**
 * @file    DummyHandler.cpp
 * @author  Alessandro Thea
 * @brief   ActionHandler class
 * @date    February 2015
 *
 */

#include "swatch/core/test/DummyHandler.hpp"
#include "swatch/core/exception.hpp"

namespace swatch {
namespace core {
namespace test {
DummyHandler::DummyHandler() :
        ActionHandler(),
        something_("something"),
        number_(0){
// Commands and Operations should be registered here
}

DummyHandler::~DummyHandler(){

}

void DummyHandler::setSomething(std::string something){
  something_ = something;
}

void DummyHandler::setNumber(uint32_t number){
  number_ = number;
}


std::string DummyHandler::something() const {
  return something_;
}

uint32_t DummyHandler::number() const {
  return number_;
}

void DummyHandler::fail() {
  throw exception("Handler failed");
}

//---
//void
//DummyHandler::registerCommand(std::string aName, Command* aCmd) {
//
//  // Check if aCmd has already been registered
//  if ( commands_.count(aName) ) {
//    // Make it an exception, because the command already exists
//    return;
//  }
//
//  commands_.emplace(aName, aCmd);
//
//}
//
//void
//DummyHandler::registerController(std::string name, Controller* ctrl) {
//
//  // Check if aCmd has already been registered
//  if ( controllers_.count(name) ) {
//    // Make it an exception, because the command already exists
//    return;
//  }
//
//  controllers_.emplace(name, ctrl);
//
//}

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
