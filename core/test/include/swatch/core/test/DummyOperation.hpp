/**
 * @file    DummyOperation.hpp
 * @author  Alessandro Thea
 * @brief   ActionableObject class
 * @date    February 2015
 *
 */

#ifndef SWATCH_CORE_TEST_DUMMYOPERATION_HPP
#define SWATCH_CORE_TEST_DUMMYOPERATION_HPP

#include "../../../../../include/swatch/core/Operation.hpp"
#include "swatch/core/ActionableObject.hpp"
//#include <xdata/Integer.h>
//#include <xdata/String.h>

namespace swatch {
namespace core {
namespace test {
class DummyOperation: public swatch::core::Operation {
public:
//  template<typename T>
  DummyOperation(  const std::string& aId );
  virtual ~DummyOperation();

  // current set of 'operations' we want the controller
  virtual void configure();
  virtual void coldReset();
  virtual void enable();
  virtual void stop();
  virtual void suspend();
  virtual void fail();
  virtual void failWithException();

  virtual bool check_true_example();
  virtual bool check_false_example();

};

//template<typename T>
//DummyOperation::DummyOperation(const std::string aId,
//    const T& aDefault, FSM* fsm) :
//        swatch::core::Operation(handler, aDefault, fsm) {
//  parameters().adopt("aa", new xdata::Integer(15));
//  parameters().adopt("todo", new xdata::String(""));
//}

} /* namespace test */
} /* namespace core */
} /* namespace swatch */

#endif /* SWATCH_CORE_TEST_DUMMYOPERATION_HPP */
