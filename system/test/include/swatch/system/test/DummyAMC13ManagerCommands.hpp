
#ifndef __SWATCH_SYSTEM_TEST_DUMMYAMC13MANAGERCOMMAND_HPP__
#define __SWATCH_SYSTEM_TEST_DUMMYAMC13MANAGERCOMMAND_HPP__


#include "swatch/core/Command.hpp"


namespace swatch {
namespace system {
namespace test {
    
    
class DummyAMC13RebootCommand : public swatch::core::Command {
public:
  DummyAMC13RebootCommand(const std::string& aId);
  ~DummyAMC13RebootCommand();
        
  virtual State code(const swatch::core::XParameterSet& );
};
    
class DummyAMC13ResetCommand : public swatch::core::Command {
public:
  DummyAMC13ResetCommand(const std::string& aId);
  ~DummyAMC13ResetCommand();
        
  virtual State code(const swatch::core::XParameterSet& );
};

class DummyAMC13ConfigureDaqCommand : public swatch::core::Command {
public:
  DummyAMC13ConfigureDaqCommand(const std::string& aId);
  ~DummyAMC13ConfigureDaqCommand();
  
  virtual State code(const swatch::core::XParameterSet& params);
};

class DummyAMC13EnableDaqCommand : public swatch::core::Command {
public:
  DummyAMC13EnableDaqCommand(const std::string& aId);
  ~DummyAMC13EnableDaqCommand();
  
  virtual State code(const swatch::core::XParameterSet& params);
};


} // namespace test
} // namespace system
} // namespace swatch

#endif /* __SWATCH_SYSTEM_TEST_DUMMYAMC13MANAGERCOMMAND_HPP__ */
