/**
 * @file    MP7Ports.hpp
 * @author  Alessandro Thea
 * @brief   MP7Processor Rx and Tx ports implementations
 * @date    February 2015
 */

#ifndef __SWATCH_MP7_MP7PORTS_HPP__
#define __SWATCH_MP7_MP7PORTS_HPP__

#include "swatch/processor/Port.hpp"


namespace  mp7 {
class MP7Controller;
class MGTRegionNode;
class DatapathNode;
class AlignMonNode;
}

namespace swatch {
namespace mp7 {

class MP7AbstractProcessor;

class MP7RxPort : public processor::InputPort {

public:
  MP7RxPort( const std::string& aId, uint32_t aChannelID, ::mp7::MP7Controller& aController );
  virtual ~MP7RxPort();

protected:
  virtual void retrieveMetricValues();
  
private:
  //! MP7 channel number
  uint32_t mChannelID;
  
  //! Reference to the MP7Controller
  ::mp7::MP7Controller& mDriver;
  
  //! Reference to Datapath node
  const ::mp7::DatapathNode& mDatapath;
  
  //! Reference to MGT Node
  const ::mp7::MGTRegionNode& mMgt;
  
  //! Reference to Alignment Node
  const ::mp7::AlignMonNode& mAlign;
  
  //! Metric containing packet counter 
  core::Metric<uint32_t>& mMetricPacketCounter;  
  
  //! Metric containing alignment bx value
  core::Metric<uint32_t>& mMetricAlignBx;

  //! Metric containing alignment cycle
  core::Metric<uint32_t>& mMetricAlignCycle;

  //! Metric containing alignment cycle
  core::Metric<uint32_t>& mMetricAlignErrors;  
};


class MP7TxPort : public processor::OutputPort {

public: 
    MP7TxPort( const std::string& aId, uint32_t aChannelID, ::mp7::MP7Controller& aController );
    virtual ~MP7TxPort();
    
protected:
    virtual void retrieveMetricValues();

private:
    //! Pointer to the main resource
    uint32_t mChannelID;
    ::mp7::MP7Controller& mDriver;
    const ::mp7::DatapathNode& mDatapath;
    const ::mp7::MGTRegionNode& mMgt;

 };
    
}
}


#endif /* __SWATCH_MP7_MP7PORTS_HPP__ */
