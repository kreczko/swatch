/* 
 * @file    MP7Commands.hpp
 * @author  Alessandro Thea
 * @date    July 2014
 *
 */

#ifndef __SWATCH_MP7_MP7COMMANDS_HPP__
#define __SWATCH_MP7_MP7COMMANDS_HPP__


#include "swatch/core/Command.hpp"

// MP7 Headers
#include "mp7/ChannelsManager.hpp"

namespace swatch {
namespace mp7 {

class MP7PortsCommand : public core::Command {
public:
  virtual ~MP7PortsCommand();
  
protected:
  template<typename T>
  MP7PortsCommand( const std::string& aId , const T& aDefault ) : core::Command(aId, aDefault) {
    registerClassParameters();
  }

  ::mp7::ChannelsManager channelsMgr( const core::XParameterSet& params );
  
private:
  
  static const std::string kPortSelection;
  void registerClassParameters();
  
};

class MP7ResetCommand : public core::Command {
public:
  MP7ResetCommand(const std::string& aId);

  virtual ~MP7ResetCommand();

  virtual State code(const core::XParameterSet& params);

};

//---
class MP7SetupMGTs : public MP7PortsCommand {
public:
  MP7SetupMGTs(const std::string& aId);
    
  virtual ~MP7SetupMGTs();

  virtual State code(const core::XParameterSet& params);

};


//---
class MP7AlignLinks : public MP7PortsCommand {
public:
  MP7AlignLinks(const std::string& aId);

  virtual ~MP7AlignLinks();

  virtual State code(const core::XParameterSet& params);
};


//---
//class MP7ConfigureLoopback : public core::Command{
//public:
//  MP7ConfigureLoopback(const std::string& aId);
//
//  virtual ~MP7ConfigureLoopback();
//
//  virtual State code(const core::XParameterSet& params);
//
//};


} /* namespace mp7 */
} /* namespace swatch */

#endif /* __SWATCH_MP7_MP7COMMANDS_HPP__ */
