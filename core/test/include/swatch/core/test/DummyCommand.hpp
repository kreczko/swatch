/**
 * @file    DummyCommand.hpp
 * @author  Tom Williams
 * @date    August 2015
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

//! Dummy command whose code method is used to test the implementation of the command base class 
class DummyCommand: public swatch::core::Command {
public:
  DummyCommand( const std::string& aId, ActionableObject& aActionable );
  virtual ~DummyCommand();

  virtual Command::State code( const XParameterSet& params); 

  static const xdata::Integer defaultResult;
  static const std::string paramX;
  static const std::string paramToDo;
  static const std::string finalMsgUseResource;
  static const std::string finalMsgSleep;
};


//! Dummy command written for unit tests; code method is a no-op that returns warning after setting status.
class DummyWarningCommand : public swatch::core::Command {
public:
  explicit DummyWarningCommand(const std::string& aId, ActionableObject& aActionable);
  virtual ~DummyWarningCommand();
  
  virtual Command::State code(const XParameterSet& params);
  
  static const xdata::Integer defaultResult;
  static const std::string finalMsg;
};


//! Dummy command written for unit tests; code method is a no-op that returns error after setting status.
class DummyErrorCommand : public swatch::core::Command {
public:
  explicit DummyErrorCommand(const std::string& aId, ActionableObject& aActionable);
  virtual ~DummyErrorCommand();
  
  virtual Command::State code(const XParameterSet& params);
  
  static const xdata::Integer defaultResult;
  static const std::string finalMsg;
  static const float finalProgress;
};


//! Dummy command written for unit tests; code method is a no-op that throws after setting progress.
class DummyThrowCommand : public swatch::core::Command {
public:
    explicit DummyThrowCommand(const std::string& aId, ActionableObject& aActionable);
    virtual ~DummyThrowCommand();
    
    virtual Command::State code(const XParameterSet& params);
    
    static const xdata::Integer defaultResult;
    static const std::string exceptionMsg;
    static const float finalProgress;
};
} /* namespace test */
} /* namespace core */
} /* namespace swatch */

#endif /* SWATCH_CORE_TEST_DUMMYCOMMAND_HPP */
