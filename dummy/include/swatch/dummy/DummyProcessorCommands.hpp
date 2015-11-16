/**
 * @file    DummyProcessorCommand.hpp
 * @author  Alessandro Thea
 * @brief   ActionableObject class
 * @date    February 2015
 *
 */

#ifndef __SWATCH_DUMMY_DUMMYPROCESSORCOMMAND_HPP__
#define __SWATCH_DUMMY_DUMMYPROCESSORCOMMAND_HPP__


#include "swatch/core/Command.hpp"


namespace swatch {
namespace dummy {

    
class DummyProcCommand : public swatch::core::Command {
public:
  DummyProcCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  ~DummyProcCommand();
  
  void sleep(const core::XParameterSet& aParams);
};

    
class DummyRebootCommand : public DummyProcCommand {
public:
  DummyRebootCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  ~DummyRebootCommand();
        
  virtual State code(const swatch::core::XParameterSet& );
};
    
class DummyResetCommand : public DummyProcCommand {
public:
  DummyResetCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  ~DummyResetCommand();
        
  virtual State code(const swatch::core::XParameterSet& );
};

class DummyConfigureTxCommand : public DummyProcCommand {
public:
  DummyConfigureTxCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  ~DummyConfigureTxCommand();
  
  virtual State code(const swatch::core::XParameterSet& params);
};

class DummyConfigureRxCommand : public DummyProcCommand {
public:
  DummyConfigureRxCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  ~DummyConfigureRxCommand();
  
  virtual State code(const swatch::core::XParameterSet& params);
};


class DummyConfigureDaqCommand : public DummyProcCommand {
public:
  DummyConfigureDaqCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  ~DummyConfigureDaqCommand();

  virtual State code(const swatch::core::XParameterSet& params);
};


class DummyConfigureAlgoCommand : public DummyProcCommand {
public:
  DummyConfigureAlgoCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  ~DummyConfigureAlgoCommand();

  State code(const swatch::core::XParameterSet& params);       
};

} // namespace dummy
} // namespace swatch

#endif /* __SWATCH_DUMMY_DUMMYPROCESSORCOMMAND_HPP__ */
