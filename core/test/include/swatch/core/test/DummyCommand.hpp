/**
 * @file    DummyCommand.hpp
 * @author  Alessandro Thea
 * @brief   ActionableObject class
 * @date    February 2015
 *
 */

#ifndef SWATCH_CORE_TEST_DUMMYCOMMAND_HPP
#define SWATCH_CORE_TEST_DUMMYCOMMAND_HPP

#include "swatch/core/Command.hpp"
#include "swatch/core/ActionableObject.hpp"
#include <xdata/Integer.h>
#include <xdata/String.h>

namespace swatch {
namespace core {
namespace test {
class DummyCommand: public swatch::core::Command {
public:
  DummyCommand(swatch::core::ActionableObject* handler);
  template<typename T>
  DummyCommand(swatch::core::ActionableObject* handler, const T& aDefault);
  virtual ~DummyCommand();

  virtual void code();


};

template<typename T>
DummyCommand::DummyCommand(swatch::core::ActionableObject* handler,
    const T& aDefault) :
        swatch::core::Command(handler, aDefault) {
  getParams().adopt("aa", new xdata::Integer(15));
  getParams().adopt("todo", new xdata::String(""));
}

} /* namespace test */
} /* namespace core */
} /* namespace swatch */

#endif /* SWATCH_CORE_TEST_DUMMYCOMMAND_HPP */
