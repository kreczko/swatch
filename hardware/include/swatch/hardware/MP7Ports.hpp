/**
 * @file    MP7Ports.hpp
 * @author  Alessandro Thea
 * @brief   MP7Processor Rx and Tx ports implementations
 * @date    February 2015
 */

#ifndef __SWATCH_HARDWARE_MP7PORTS_HPP__
#define __SWATCH_HARDWARE_MP7PORTS_HPP__

#include "swatch/core/Port.hpp"

namespace  mp7 {
class MP7Controller;
class MGTRegionNode;
class CtrlNode;
class AlignMonNode;
}

namespace swatch {
namespace hardware {

class MP7Processor;

class MP7RxPort : public core::InputPort {
public:

  MP7RxPort(const std::string& aId, uint32_t aChannelID, mp7::MP7Controller* aProcessor );
  virtual ~MP7RxPort();
  
  virtual bool isEnabled() const;
  virtual bool isOperating() const;
  virtual bool isAligned() const;
  virtual uint32_t getCRCErrors() const;


private:
  //! Pointer to the main resource
  uint32_t channelID_;
  mp7::MP7Controller* driver_;
  const mp7::CtrlNode& ctrl_;
  const mp7::MGTRegionNode& mgt_;
  const mp7::AlignMonNode& align_;

};
    
}
}


#endif /* __SWATCH_HARDWARE_MP7PORTS_HPP__ */