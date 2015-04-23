/**
 * @file    MP7Ports.cpp
 * @author  Alessandro Thea
 * @brief   MP7Processor Rx and Tx ports implementations
 * @date    February 2015
 */


#include "swatch/hardware/MP7Ports.hpp"

// MP7 Headers
#include "mp7/MP7Controller.hpp"
#include "mp7/DatapathNode.hpp"
#include "mp7/MGTRegionNode.hpp"
#include "mp7/ChannelIDSet.hpp"
#include "mp7/AlignMonNode.hpp"
#include "mp7/Utilities.hpp"

namespace swatch {
namespace hardware {

//---
MP7RxPort::MP7RxPort( const std::string& aId, uint32_t aChannelID, MP7Processor& aProcessor ) :
  InputPort(aId),
  channelID_(aChannelID),
  processor_(aProcessor),
  driver_(aProcessor.driver()),
  datapath_(aProcessor.driver().getDatapath()),
  mgt_(datapath_.getNode<mp7::MGTRegionNode>("region.mgt")),
  align_(datapath_.getNode<mp7::AlignMonNode>("region.align")) {
}


//---
MP7RxPort::~MP7RxPort() {
}


//---
bool
MP7RxPort::isEnabled() const {
  // Where to store the information if the port is enabled or not?
  // In the MP7 case is slightly complicated. The Masks will be known to the processor, but not to the driver.
  // Interesting problem
  return true;
  
}


//---
bool
MP7RxPort::isLocked() const {
  datapath_.selectLink(channelID_);
  
  // Calculate the channel local id
  uint32_t l = mp7::ChannelIDSet::channelToLocal(channelID_);

  // Point to the right node
  std::string path = mp7::strprintf("ro_regs.ch%d.status", l);

  // Get all subregisters
  mp7::Snapshot s =  mp7::snapshot(mgt_.getNode(path));

  return (
    // Not in reset
    !s["rxusrrst"] and
    // Reset completed
    s["rx_fsm_reset_done"] and
    // And no crcs
    s["crc_checked"] != 0x0
  );
}


//---
bool MP7RxPort::isAligned() const {
  datapath_.selectLink(channelID_);
  return ( align_.errors() == 0  );
}


//---
uint32_t
MP7RxPort::getCRCErrors() const {
  datapath_.selectLink(channelID_);
  return mgt_.readCrcErrors(mp7::ChannelIDSet::channelToLocal(channelID_));
}


//---
MP7TxPort::MP7TxPort(const std::string& aId, uint32_t aChannelID, MP7Processor& aProcessor) :
  OutputPort(aId),
  channelID_(aChannelID),
  processor_(aProcessor),
  driver_(aProcessor.driver()),
  datapath_(aProcessor.driver().getDatapath()),
  mgt_(datapath_.getNode<mp7::MGTRegionNode>("region.mgt")) {

}

//---
MP7TxPort::~MP7TxPort() {
}


//---
bool MP7TxPort::isEnabled() const {
  return true;
}


//---
bool MP7TxPort::isOperating() const {
  datapath_.selectLink(channelID_);
  
  // Calculate the channel local id
  uint32_t l = mp7::ChannelIDSet::channelToLocal(channelID_);

  // Point to the right node
  std::string path = mp7::strprintf("ro_regs.ch%d.status", l);

  // Get all subregisters
  mp7::Snapshot s =  mp7::snapshot(mgt_.getNode(path));

  return (
    // Not in reset
    !s["txusrrst"] and
    // Reset completed
    s["tx_fsm_reset_done"]
  );
}


}
}
