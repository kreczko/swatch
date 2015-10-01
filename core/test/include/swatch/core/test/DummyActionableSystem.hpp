/* 
 * File:   DummYActionableSystem.hpp
 * Author: tom
 *
 * Created on 18 September 2015, 16:22
 */

#ifndef __SWATCH_CORE_TEST_DUMMYACTIONABLESYSTEM_HPP__
#define	__SWATCH_CORE_TEST_DUMMYACTIONABLESYSTEM_HPP__


#include "swatch/core/ActionableSystem.hpp"


namespace swatch{
namespace core{
namespace test{

class DummyActionableSystem : public ActionableSystem
{
public:
  DummyActionableSystem(const std::string& aId);

  virtual ~DummyActionableSystem();

  // Expose protected method as public for unit testing ...
  SystemStateMachine& registerStateMachine(const std::string& aId, const std::string& aInitialState, const std::string& aErrorState );

  // Expose protected method as public for unit testing ...
  template< typename ObjType, typename DeleterType>
  ObjType& add( ObjType* aChild , DeleterType aDeleter);
};


template< typename ObjType, typename DeleterType>
ObjType& DummyActionableSystem::add( ObjType* aChild , DeleterType aDeleter)
{ 
  const std::string& childId = aChild->getId();
  this->Object::addObj(aChild, aDeleter);
  return *getObj<ObjType>(childId);
}

}
}
}


#endif	/* __SWATCH_CORE_TEST_DUMMYACTIONABLESYSTEM_HPP__ */

