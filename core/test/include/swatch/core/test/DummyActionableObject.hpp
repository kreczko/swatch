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

  DummyActionableObject(const std::string& aId);

  virtual ~DummyActionableObject();

  uint32_t getNumber() const;

  void setNumber(uint32_t number);

  std::string something() const;

  void setSomething(std::string something);

  void fail();
  
  // Expose registerFunctionoid template method as public for tests
  template< typename T>
  T& registerFunctionoid( const std::string& aId ) { return ActionableObject::registerFunctionoid<T>(aId); }

  template< typename ObjType, typename DeleterType>
  ObjType& add( ObjType* aChild , DeleterType aDeleter);

private:

  void retrieveMetricValues() {}
    
  std::string something_;
  uint32_t number_;
  
  DummyActionableObject( const DummyActionableObject& other ); // non copyable
  DummyActionableObject& operator=( const DummyActionableObject& ); // non copyable

};

template< typename ObjType, typename DeleterType>
ObjType& DummyActionableObject::add( ObjType* aChild , DeleterType aDeleter)
{ 
  const std::string& childId = getId();
  this->Object::addObj(aChild, aDeleter);
  return *getObj<ObjType>(childId);
}

} /* namespace test */
} /* namespace core */
} /* namespace swatch */

#endif /* SWATCH_CORE_TEST_DUMMYACTIONABLEOBJECT_HPP */