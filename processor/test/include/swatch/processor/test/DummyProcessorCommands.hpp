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

    
class DummyProcCommand : public swatch::core::Command {
public:
  DummyProcCommand(const std::string& aId);
  ~DummyProcCommand();
  
  void sleep(const core::XParameterSet& aParams);
};

    
class DummyRebootCommand : public DummyProcCommand {
public:
  DummyRebootCommand(const std::string& aId);
  ~DummyRebootCommand();
        
  virtual State code(const swatch::core::XParameterSet& );
};
    
class DummyResetCommand : public DummyProcCommand {
public:
  DummyResetCommand(const std::string& aId);
  ~DummyResetCommand();
        
  virtual State code(const swatch::core::XParameterSet& );
};

class DummyConfigureTxCommand : public DummyProcCommand {
public:
  DummyConfigureTxCommand(const std::string& aId);
  ~DummyConfigureTxCommand();
  
  virtual State code(const swatch::core::XParameterSet& params);
};

class DummyConfigureRxCommand : public DummyProcCommand {
public:
  DummyConfigureRxCommand(const std::string& aId);
  ~DummyConfigureRxCommand();
  
  virtual State code(const swatch::core::XParameterSet& params);
};


class DummyConfigureDaqCommand : public DummyProcCommand {
public:
  DummyConfigureDaqCommand(const std::string& aId);
  ~DummyConfigureDaqCommand();

  virtual State code(const swatch::core::XParameterSet& params);
};


class DummyConfigureAlgoCommand : public DummyProcCommand {
public:
  DummyConfigureAlgoCommand(const std::string& aId);
  ~DummyConfigureAlgoCommand();

  State code(const swatch::core::XParameterSet& params);       
};

} // namespace test
} // namespace processor
} // namespace swatch

#endif /* __SWATCH_PROCESSOR_TEST_DUMMYPROCESSORCOMMAND_HPP__ */
