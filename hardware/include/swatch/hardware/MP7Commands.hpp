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
  MP7ResetCommand(const std::string& aId);
  virtual ~MP7ResetCommand();

  virtual void code();

};


class MP7ConfigureLoopback : public core::Command{
public:
  MP7ConfigureLoopback(const std::string& aId);

  virtual ~MP7ConfigureLoopback();

  virtual void code();

};


} /* namespace hardware */
} /* namespace swatch */

#endif /* __SWATCH_HARDWARE_MP7COMMANDS_HPP__ */