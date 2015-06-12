/**
 * @file    MP7Ports.hpp
 * @author  Alessandro Thea
 * @brief   MP7Processor Rx and Tx ports implementations
 * @date    February 2015
 */

#ifndef __SWATCH_HARDWARE_MP7PORTS_HPP__
#define __SWATCH_HARDWARE_MP7PORTS_HPP__

#include "swatch/processor/Port.hpp"


namespace  mp7 {
class MP7Controller;
class MGTRegionNode;
class DatapathNode;
class AlignMonNode;
}

namespace swatch {
namespace hardware {

class MP7Processor;

class MP7RxPort : public processor::InputPort {

public:
  MP7RxPort( const std::string& aId, uint32_t aChannelID, MP7Processor& aProcessor );
  virtual ~MP7RxPort();

protected:
  virtual void implementUpdateMetrics();
  
private:
  //! Pointer to the main resource
  uint32_t channelID_;
  MP7Processor& processor_;
  mp7::MP7Controller& driver_;
//  const mp7::CtrlNode& ctrl_;
  const mp7::DatapathNode& datapath_;
  const mp7::MGTRegionNode& mgt_;
  const mp7::AlignMonNode& align_;
};


class MP7TxPort : public processor::OutputPort {

public: 
    MP7TxPort( const std::string& aId, uint32_t aChannelID, MP7Processor& aProcessor );
    virtual ~MP7TxPort();
    
protected:
    virtual void implementUpdateMetrics();

private:
    //! Pointer to the main resource
    uint32_t channelID_;
    MP7Processor& processor_;
    mp7::MP7Controller& driver_;
//    const mp7::CtrlNode& ctrl_;
    const mp7::DatapathNode& datapath_;
    const mp7::MGTRegionNode& mgt_;

 };
    
}
}


#endif /* __SWATCH_HARDWARE_MP7PORTS_HPP__ */
