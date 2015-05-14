/**
 * @file    DummyHandler.hpp
 * @author  Alessandro Thea
 * @brief   ActionableObject class
 * @date    February 2015
 *
 */

#ifndef SWATCH_CORE_TEST_DUMMYHANDLER_HPP
#define SWATCH_CORE_TEST_DUMMYHANDLER_HPP

#include "swatch/core/ActionableObject.hpp"

namespace swatch {
namespace core {
namespace test {

class DummyHandler: public swatch::core::ActionableObject {
public:
  DummyHandler();

  virtual ~DummyHandler();

//  void Register(std::string aName, Command* aCmd);
//  void registerController(std::string name, Controller* ctrl);

  uint32_t number() const;

  void setNumber(uint32_t number);

  std::string something() const;

  void setSomething(std::string something);

  void fail();

private:
  std::string something_;
  uint32_t number_;
};

} /* namespace test */
} /* namespace core */
} /* namespace swatch */

#endif /* SWATCH_CORE_TEST_DUMMYHANDLER_HPP */
