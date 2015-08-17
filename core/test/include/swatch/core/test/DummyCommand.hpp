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
  DummyCommand( const std::string& aId );
  template<typename T>  DummyCommand( const std::string& aId , const T& aDefault);
  virtual ~DummyCommand();

  virtual Command::State code( const XParameterSet& params); 

  static const xdata::Integer defaultResult;
  static const std::string finalMsgUseResource;
  static const std::string finalMsgSleep;
  static const std::string finalMsgWarning;
  static const std::string finalMsgError;
  static const std::string exceptionMsg;
  static const float finalProgressError;
  static const float finalProgressThrow;  
};

template<typename T>
DummyCommand::DummyCommand( const std::string& aId , const T& aDefault) :
       swatch::core::Command( aId , aDefault) {
  registerParameter("x", xdata::Integer(defaultResult));
  registerParameter("todo", xdata::String(""));
}

} /* namespace test */
} /* namespace core */
} /* namespace swatch */

#endif /* SWATCH_CORE_TEST_DUMMYCOMMAND_HPP */
