/**
 * @file    DummyProcessorCommand.hpp
 * @author  Alessandro Thea
 * @brief   ActionableObject class
 * @date    February 2015
 *
 */

#ifndef __SWATCH_PROCESSOR_TEST_DUMMYPROCESSORCOMMAND_HPP__
#define __SWATCH_PROCESSOR_TEST_DUMMYPROCESSORCOMMAND_HPP__

#include "swatch/core/Command.hpp"
#include "swatch/core/ActionableObject.hpp"
#include <xdata/Integer.h>
#include <xdata/String.h>
namespace swatch {
namespace processor {
namespace test {
class DummyProcessorCommand: public swatch::core::Command {
public:
  DummyProcessorCommand(swatch::core::ActionableObject* handler);
  template<typename T>
  DummyProcessorCommand(swatch::core::ActionableObject* handler, const T& aDefault);
  virtual ~DummyProcessorCommand();

  virtual void code();


};

template<typename T>
DummyProcessorCommand::DummyProcessorCommand(swatch::core::ActionableObject* handler,
    const T& aDefault) :
        swatch::core::Command(handler, aDefault) {
  getParams().adopt("aa", new xdata::Integer(15));
  getParams().adopt("todo", new xdata::String(""));
}
} // namespace test
} // namespace processor
} // namespace swatch
#endif /* __SWATCH_PROCESSOR_TEST_DUMMYPROCESSORCOMMAND_HPP__ */
