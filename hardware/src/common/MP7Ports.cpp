/**
 * @file    MP7Ports.cpp
 * @author  Alessandro Thea
 * @brief   MP7Processor Rx and Tx ports implementations
 * @date    February 2015
 */

#include "swatch/hardware/MP7Ports.hpp"


// SWATCH headers
#include "swatch/hardware/MP7Processor.hpp"

// MP7 headers
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
  align_(datapath_.getNode<mp7::AlignMonNode>("region.align"))
{
}


//---
MP7RxPort::~MP7RxPort() {
}


//---
void MP7RxPort::implementUpdateMetrics()
{
  // Where to store the information if the port is enabled or not?
  // In the MP7 case is slightly complicated. The Masks will be known to the processor, but not to the driver.
  // Interesting problem
  setMetricValue<>(metricIsEnabled_, true);
  
  // Select the link, and calculate channel's local ID (within quad) ...
  datapath_.selectLink(this->channelID_);
  uint32_t localId = mp7::ChannelIDSet::channelToLocal(channelID_);
  

  /* IS LOCKED */
  // Point to the right node
  std::string mgtStatusNodePath = "ro_regs.ch" + boost::lexical_cast<std::string>(localId) + ".status";
  uhal::ValWord<uint32_t> mgtInReset = mgt_.getNode(mgtStatusNodePath+".rxusrrst").read();
  uhal::ValWord<uint32_t> mgtResetDone = mgt_.getNode(mgtStatusNodePath+".rx_fsm_reset_done").read();
  uhal::ValWord<uint32_t> mgtNoCRCs = mgt_.getNode(mgtStatusNodePath+".crc_checked").read();

  /* IS ALIGNED */
  uhal::ValWord<uint32_t> alignErrors = align_.getNode("stat.err_cnt").read();
  
  
  /* CRC ERRORS */
  //if (aChannel > 3)
  //  throw mp7:MGTChannelIdOutOfBounds("Invalid channel number requested");
  uhal::ValWord<uint32_t> crcErrors = mgt_.getNode(mgtStatusNodePath+".crc_error").read();
  driver_.hwInterface().dispatch();

  bool isLocked (
    // Not in reset
    !mgtInReset.value() and
    // Reset completed
    mgtResetDone.value() and
    // And no crcs
    mgtNoCRCs.value() != 0x0
  );

  setMetricValue<>(metricIsLocked_, isLocked);
  
  setMetricValue<>(metricIsAligned_, alignErrors.value() == 0);
  
  setMetricValue<>(metricCRCErrors_, crcErrors.value());
}



//---
MP7TxPort::MP7TxPort(const std::string& aId, uint32_t aChannelID, MP7Processor& aProcessor) :
  OutputPort(aId),
  channelID_(aChannelID),
  processor_(aProcessor),
  driver_(aProcessor.driver()),
  datapath_(aProcessor.driver().getDatapath()),
  mgt_(datapath_.getNode<mp7::MGTRegionNode>("region.mgt"))
{
}


//---
MP7TxPort::~MP7TxPort() {
}


//---
void MP7TxPort::implementUpdateMetrics()
{  
  setMetricValue<>(metricIsEnabled_, true);
  
  /* IS OPERATING */
  datapath_.selectLink(channelID_);
  
  // Calculate the channel local id
  uint32_t l = mp7::ChannelIDSet::channelToLocal(channelID_);

  // Point to the right node
  std::string path = mp7::strprintf("ro_regs.ch%d.status", l);

  // Get all subregisters
  mp7::Snapshot s =  mp7::snapshot(mgt_.getNode(path));

  bool isOperating (
    // Not in reset
    !s["txusrrst"] and
    // Reset completed
    s["tx_fsm_reset_done"]
  );

  setMetricValue<>(metricIsOperating_, isOperating);
}

}
}
