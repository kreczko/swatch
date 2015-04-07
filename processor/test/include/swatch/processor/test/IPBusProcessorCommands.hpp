/**
 * @file    IPBusProcessor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef __SWATCH_CORE_TEST_IPBUSPROCESSORCOMMAND__
#define __SWATCH_CORE_TEST_IPBUSPROCESSORCOMMAND__

#include "swatch/core/Command.hpp"
namespace swatch {

namespace core {
class ActionHandler;
}

namespace processor {
namespace test {

class IPBusResetCommand : public swatch::core::Command {
public:
    IPBusResetCommand(core::ActionHandler* aHandler);

  virtual ~IPBusResetCommand();

  virtual void code();

};

class IPBusConfigureCommand : public swatch::core::Command {
public:
    IPBusConfigureCommand(core::ActionHandler* aHandler);

  virtual ~IPBusConfigureCommand();

  virtual void code();

};


class IPBusCapture : public swatch::core::Command {
public:
    IPBusCapture(core::ActionHandler* aHandler);

  virtual ~IPBusCapture();

  virtual void code();

};




} // namespace test
} // namespace processor
} // namespace swatch

 #endif /* __SWATCH_CORE_TEST_IPBUSPROCESSORCOMMAND__ */