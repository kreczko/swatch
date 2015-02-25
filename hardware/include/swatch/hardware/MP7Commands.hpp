/* 
 * @file    MP7Commands.hpp
 * @author  Alessandro Thea
 * @date    July 2014
 *
 */

#ifndef __SWATCH_HARDWARE_MP7COMMANDS_HPP__
#define __SWATCH_HARDWARE_MP7COMMANDS_HPP__

#include "swatch/core/Command.hpp"

namespace swatch {
namespace hardware {

class MP7ResetCommand : public core::Command {
public:
  MP7ResetCommand( core::ActionHandler* aHandler );
  virtual ~MP7ResetCommand();

  virtual void exec();

private:

};

} /* namespace hardware */
} /* namespace swatch */

#endif /* __SWATCH_HARDWARE_MP7COMMANDS_HPP__ */