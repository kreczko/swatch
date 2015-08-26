/**
 * @file    DummyActionableObject.cpp
 * @author  Alessandro Thea
 * @date    February 2015
 *
 */

#include "swatch/core/test/DummyActionableObject.hpp"


#include "swatch/core/exception.hpp"


namespace swatch {
namespace core {
namespace test {


DummyActionableObject::DummyActionableObject() :
  ActionableObject("Dummy"),
  something_("something"),
  number_(0) {
// Commands and Operations should be registered here
}



DummyActionableObject::DummyActionableObject(const std::string& aId) :
  ActionableObject(aId),
  something_("something"),
  number_(0) {
// Commands and Operations should be registered here
}


DummyActionableObject::~DummyActionableObject(){

}


void DummyActionableObject::setSomething(std::string something){
  something_ = something;
}


void DummyActionableObject::setNumber(uint32_t number){
  number_ = number;
}


std::string DummyActionableObject::something() const {
  return something_;
}


uint32_t DummyActionableObject::getNumber() const {
  return number_;
}


void DummyActionableObject::fail() {
  throw exception("DummyActionableObject failed");
}


} /* namespace test */
} /* namespace core */
} /* namespace swatch */
