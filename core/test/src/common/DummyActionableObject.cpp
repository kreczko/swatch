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

const std::vector<std::string>& DummyActionableObject::getGateKeeperTables() const
{
  // Can't set the table names in constructor, since don't know parent at that time ...
  // ... instead, have to set tables names first time this method is called
  if( gateKeeperTables_.empty() )
  {
    gateKeeperTables_.push_back(getPath());

    std::string basePath = getPath();
    basePath.resize(basePath.size() - getId().size());
    gateKeeperTables_.push_back(basePath + "common");
  }
  return gateKeeperTables_;
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
