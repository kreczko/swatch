/**
 * @file    DummyProcessorCommand.hpp
 * @author  Alessandro Thea
 * @brief   ActionableObject class
 * @date    February 2015
 *
 */

#ifndef __SWATCH_PROCESSOR_TEST_DUMMYPROCESSORCOMMAND_HPP__
#define __SWATCH_PROCESSOR_TEST_DUMMYPROCESSORCOMMAND_HPP__

#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/core/ActionableObject.hpp"
#include <xdata/Integer.h>
#include <xdata/String.h>
namespace swatch {
namespace processor {
namespace test {
class DummyProcessorCommand: public swatch::core::test::DummyCommand {
public:
  DummyProcessorCommand(const std::string& aId);
  template<typename T>
  DummyProcessorCommand(const std::string& aId, const T& aDefault);
  virtual ~DummyProcessorCommand();

  virtual State code(const swatch::core::XParameterSet& params);
};

template<typename T>
DummyProcessorCommand::DummyProcessorCommand(const std::string& aId,
    const T& aDefault) :
        swatch::core::Command(aId, aDefault) {
  registerParameter("aa", xdata::Integer(15));
  registerParameter("todo", xdata::String(""));
}
} // namespace test
} // namespace processor
} // namespace swatch
#endif /* __SWATCH_PROCESSOR_TEST_DUMMYPROCESSORCOMMAND_HPP__ */
