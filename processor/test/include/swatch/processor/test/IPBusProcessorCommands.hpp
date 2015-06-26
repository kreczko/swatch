/**
 * @file    IPBusProcessor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef __SWATCH_CORE_TEST_IPBUSPROCESSORCOMMAND__
#define __SWATCH_CORE_TEST_IPBUSPROCESSORCOMMAND__

#include "swatch/core/test/DummyCommand.hpp"
namespace swatch {

namespace core {
class ActionableObject;
}

namespace processor {
namespace test {

class IPBusResetCommand : public swatch::core::test::DummyCommand {
public:
    IPBusResetCommand(const std::string& aId);

  virtual ~IPBusResetCommand();

  virtual State code();

};

class IPBusConfigureCommand : public swatch::core::test::DummyCommand {
public:
    IPBusConfigureCommand(const std::string& aId);

  virtual ~IPBusConfigureCommand();

  virtual State code();

};


class IPBusCapture : public swatch::core::test::DummyCommand {
public:
    IPBusCapture(const std::string& aId);

  virtual ~IPBusCapture();

  virtual State code();

};




} // namespace test
} // namespace processor
} // namespace swatch

 #endif /* __SWATCH_CORE_TEST_IPBUSPROCESSORCOMMAND__ */
