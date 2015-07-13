/**
 * @file    DummyActionableObject.hpp
 * @author  Alessandro Thea
 * @date    February 2015
 *
 */

#ifndef SWATCH_CORE_TEST_DUMMYACTIONABLEOBJECT_HPP
#define SWATCH_CORE_TEST_DUMMYACTIONABLEOBJECT_HPP


#include "swatch/core/ActionableObject.hpp"


namespace swatch {
namespace core {
namespace test {

class DummyActionableObject: public swatch::core::ActionableObject {
public:
  DummyActionableObject();

  virtual ~DummyActionableObject();

  uint32_t number() const;

  void setNumber(uint32_t number);

  std::string something() const;

  void setSomething(std::string something);

  void fail();

private:

  void implementUpdateMetrics() {}
    
  std::string something_;
  uint32_t number_;
  
  DummyActionableObject( const DummyActionableObject& other ); // non copyable
  DummyActionableObject& operator=( const DummyActionableObject& ); // non copyable

};

} /* namespace test */
} /* namespace core */
} /* namespace swatch */

#endif /* SWATCH_CORE_TEST_DUMMYACTIONABLEOBJECT_HPP */
