/**
 * @file    MP7Ports.cpp
 * @author  Alessandro Thea
 * @brief   MP7Processor Rx and Tx ports implementations
 * @date    February 2015
 */


#include "swatch/hardware/MP7Ports.hpp"

// MP7 Headers
#include "mp7/MP7Controller.hpp"
#include "mp7/CtrlNode.hpp"
#include "mp7/MGTRegionNode.hpp"
#include "mp7/ChannelIDSet.hpp"
#include "mp7/AlignMonNode.hpp"

namespace swatch {
namespace hardware {

//---
MP7RxPort::MP7RxPort( const std::string& aId, uint32_t aChannelID, mp7::MP7Controller* aDriver ) :
  InputPort(aId),
  channelID_(aChannelID),
  driver_(aDriver),
  ctrl_(aDriver->getCtrl()),
  mgt_(driver_->hwInterface().getNode<mp7::MGTRegionNode>("datapath.region.mgt")),
  align_(driver_->hwInterface().getNode<mp7::AlignMonNode>("datapath.region.align")){
}


//---
MP7RxPort::~MP7RxPort() {
}

bool
MP7RxPort::isEnabled() const {
  // Where to store the information if the port is enabled or not?
  // In the MP7 case is slightly complicated. The Masks will be known to the processor, but not to the driver.
  // Interesting problem
  return true;
  
}

bool
MP7RxPort::isOperating() const {
  ctrl_.selectLink(channelID_);
  
  return ( mgt_.crcChecked(mp7::ChannelIDSet::channelToLocal(channelID_)) != 0 );
}

bool MP7RxPort::isAligned() const {
  ctrl_.selectLink(channelID_);
  return ( align_.errors() == 0  );
}

uint32_t
MP7RxPort::getCRCErrors() const {
  ctrl_.selectLink(channelID_);
  return mgt_.crcErrors(mp7::ChannelIDSet::channelToLocal(channelID_));
}


}
}
