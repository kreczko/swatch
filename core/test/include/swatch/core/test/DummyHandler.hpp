/**
 * @file    DummyHandler.hpp
 * @author  Alessandro Thea
 * @brief   ActionHandler class
 * @date    February 2015
 *
 */

#ifndef SWATCH_CORE_TEST_DUMMYHANDLER_HPP
#define SWATCH_CORE_TEST_DUMMYHANDLER_HPP

#include "swatch/core/ActionHandler.hpp"

class Other {

};

namespace swatch {
namespace core {
namespace test {

class DummyHandler: public Other, public swatch::core::ActionHandler {
public:
  DummyHandler();

  virtual ~DummyHandler();

//  void registerCommand(std::string aName, Command* aCmd);
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