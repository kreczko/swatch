/* 
 * @file    MP7Commands.hpp
 * @author  Alessandro Thea
 * @date    July 2014
 *
 */

#ifndef __SWATCH_MP7_MP7COMMANDS_HPP__
#define __SWATCH_MP7_MP7COMMANDS_HPP__


#include "swatch/core/Command.hpp"


namespace swatch {
namespace mp7 {

class MP7ResetCommand : public core::Command {
public:
  MP7ResetCommand(const std::string& aId);

  virtual ~MP7ResetCommand();

  virtual State code(const core::XParameterSet& params);

};

//---
class MP7SetupLinks : public core::Command {
public:
  MP7SetupLinks(const std::string& aId);
    
  virtual ~MP7SetupLinks();

  virtual State code(const core::XParameterSet& params);

};


//---
class MP7AlignLinks : public core::Command {
public:
  MP7AlignLinks(const std::string& aId);

  virtual ~MP7AlignLinks();

  virtual State code(const core::XParameterSet& params);
};


//---
class MP7ConfigureLoopback : public core::Command{
public:
  MP7ConfigureLoopback(const std::string& aId);

  virtual ~MP7ConfigureLoopback();

  virtual State code(const core::XParameterSet& params);

};


} /* namespace mp7 */
} /* namespace swatch */

#endif /* __SWATCH_MP7_MP7COMMANDS_HPP__ */
