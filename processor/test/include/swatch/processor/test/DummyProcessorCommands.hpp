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
    
    
class DummyRebootCommand : public swatch::core::Command {
public:
  DummyRebootCommand(const std::string& aId);
  ~DummyRebootCommand();
        
  virtual State code(const swatch::core::XParameterSet& );
};
    
class DummyResetCommand : public swatch::core::Command {
public:
  DummyResetCommand(const std::string& aId);
  ~DummyResetCommand();
        
  virtual State code(const swatch::core::XParameterSet& );
};

class DummyConfigureTxCommand : public swatch::core::Command {
public:
  DummyConfigureTxCommand(const std::string& aId);
  ~DummyConfigureTxCommand();
  
  virtual State code(const swatch::core::XParameterSet& params);
};

class DummyConfigureRxCommand : public swatch::core::Command {
public:
  DummyConfigureRxCommand(const std::string& aId);
  ~DummyConfigureRxCommand();
  
  virtual State code(const swatch::core::XParameterSet& params);
};


class DummyConfigureDaqCommand : public swatch::core::Command {
public:
  DummyConfigureDaqCommand(const std::string& aId);
  ~DummyConfigureDaqCommand();

  virtual State code(const swatch::core::XParameterSet& params);
};


class DummyConfigureAlgoCommand : public swatch::core::Command {
public:
  DummyConfigureAlgoCommand(const std::string& aId);
  ~DummyConfigureAlgoCommand();

  State code(const swatch::core::XParameterSet& params);       
};

} // namespace test
} // namespace processor
} // namespace swatch

#endif /* __SWATCH_PROCESSOR_TEST_DUMMYPROCESSORCOMMAND_HPP__ */
