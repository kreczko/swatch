/**
 * @file    DummyCommand.hpp
 * @author  Alessandro Thea
 * @brief   ActionHandler class
 * @date    February 2015
 *
 */

#ifndef SWATCH_CORE_TEST_DUMMYCOMMAND_HPP
#define SWATCH_CORE_TEST_DUMMYCOMMAND_HPP

#include "swatch/core/Command.hpp"
#include "swatch/core/ActionHandler.hpp"
#include <xdata/Integer.h>
#include <xdata/String.h>

namespace swatch {
namespace core {
namespace test {
class DummyCommand: public swatch::core::Command {
public:
  DummyCommand(swatch::core::ActionHandler* handler);
  template<typename T>
  DummyCommand(swatch::core::ActionHandler* handler, const T& aDefault);
  virtual ~DummyCommand();

  virtual void code();


};

template<typename T>
DummyCommand::DummyCommand(swatch::core::ActionHandler* handler,
    const T& aDefault) :
        swatch::core::Command(handler, aDefault) {
  parameters().adopt("aa", new xdata::Integer(15));
  parameters().adopt("todo", new xdata::String(""));
}

} /* namespace test */
} /* namespace core */
} /* namespace swatch */

#endif /* SWATCH_CORE_TEST_DUMMYCOMMAND_HPP */
