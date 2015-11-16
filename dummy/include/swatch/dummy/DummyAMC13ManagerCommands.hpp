
#ifndef __SWATCH_DUMMY_DUMMYAMC13MANAGERCOMMAND_HPP__
#define __SWATCH_DUMMY_DUMMYAMC13MANAGERCOMMAND_HPP__


#include "swatch/core/Command.hpp"


namespace swatch {
namespace dummy {


class DummyAMC13Command : public swatch::core::Command {
public:
  DummyAMC13Command(const std::string& aId, swatch::core::ActionableObject& aActionable);
  ~DummyAMC13Command();
  
  void sleep(const core::XParameterSet& aParams);
};


class DummyAMC13RebootCommand : public DummyAMC13Command {
public:
  DummyAMC13RebootCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  ~DummyAMC13RebootCommand();
        
  virtual State code(const swatch::core::XParameterSet& );
};


class DummyAMC13ResetCommand : public DummyAMC13Command {
public:
  DummyAMC13ResetCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  ~DummyAMC13ResetCommand();
        
  virtual State code(const swatch::core::XParameterSet& );
};


class DummyAMC13ConfigureDaqCommand : public DummyAMC13Command {
public:
  DummyAMC13ConfigureDaqCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  ~DummyAMC13ConfigureDaqCommand();
  
  virtual State code(const swatch::core::XParameterSet& params);
};


class DummyAMC13StartDaqCommand : public DummyAMC13Command {
public:
  DummyAMC13StartDaqCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  ~DummyAMC13StartDaqCommand();
  
  virtual State code(const swatch::core::XParameterSet& params);
};


class DummyAMC13StopDaqCommand : public DummyAMC13Command {
public:
  DummyAMC13StopDaqCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  ~DummyAMC13StopDaqCommand();
  
  virtual State code(const swatch::core::XParameterSet& params);
};


} // namespace dummy
} // namespace swatch

#endif /* __SWATCH_DUMMY_DUMMYAMC13MANAGERCOMMAND_HPP__ */
