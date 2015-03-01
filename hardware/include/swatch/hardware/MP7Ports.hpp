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
}

namespace swatch {
namespace hardware {

class MP7Processor;

class MP7RxPort : public core::InputPort {
public:

  MP7RxPort(const std::string& aId, mp7::MP7Controller* aProcessor );
  virtual ~MP7RxPort();
  
  virtual uint32_t getErrors() const;
  virtual bool isOperating() const;
  virtual bool isEnabled() const;

private:
  //! Pointer to the main resource
  mp7::MP7Controller* driver_;

};
    
}
}


#endif /* __SWATCH_HARDWARE_MP7PORTS_HPP__ */