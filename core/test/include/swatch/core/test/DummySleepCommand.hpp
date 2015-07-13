/**
 * @file    DummySleepCommand.hpp
 * @author  Tom Williams
 * @date    February 2015
 */

#ifndef SWATCH_CORE_TEST_DUMMYSLEEPCOMMAND_HPP
#define SWATCH_CORE_TEST_DUMMYSLEEPCOMMAND_HPP

#include "swatch/core/Command.hpp"

#include "xdata/Integer.h"
#include "xdata/String.h"

namespace swatch {
namespace core {
namespace test {


class DummySleepCommand: public swatch::core::Command {
public:
  DummySleepCommand( const std::string& aId );

  virtual ~DummySleepCommand();

  virtual Command::State code(const XParameterSet& params); 
};


} /* namespace test */
} /* namespace core */
} /* namespace swatch */

#endif /* SWATCH_CORE_TEST_DUMMYCOMMAND_HPP */
