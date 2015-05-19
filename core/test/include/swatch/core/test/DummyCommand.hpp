/**
 * @file    DummyCommand.hpp
 * @author  Alessandro Thea
 * @brief   ActionableObject class
 * @date    February 2015
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
class DummyCommand: public swatch::core::Command {
public:
  DummyCommand( const std::string& aId );
  template<typename T>  DummyCommand( const std::string& aId , const T& aDefault);
  virtual ~DummyCommand();

  virtual void code( XParameterSet& params); ///Should take const reference but xdata::serializable is const-correctness broken

  template<typename T>
  void registerParam(const std::string& name, const T& defaultValue) {
    core::Functionoid::registerParameter(name, defaultValue);
  }
};

template<typename T>
DummyCommand::DummyCommand( const std::string& aId , const T& aDefault) :
       swatch::core::Command( aId , aDefault) {
  registerParameter("aa", xdata::Integer(15));
  registerParameter("todo", xdata::String(""));
}

} /* namespace test */
} /* namespace core */
} /* namespace swatch */

#endif /* SWATCH_CORE_TEST_DUMMYCOMMAND_HPP */
