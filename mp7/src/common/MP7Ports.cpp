/**
 * @file    MP7Ports.cpp
 * @author  Alessandro Thea
 * @brief   MP7Processor Rx and Tx ports implementations
 * @date    February 2015
 */

#include "swatch/mp7/MP7Ports.hpp"


// SWATCH headers
#include "swatch/mp7/MP7AbstractProcessor.hpp"

// MP7 headers
#include "mp7/MP7Controller.hpp"
#include "mp7/DatapathNode.hpp"
#include "mp7/MGTRegionNode.hpp"
#include "mp7/AlignMonNode.hpp"
#include "mp7/Utilities.hpp"
#include "swatch/core/MetricConditions.hpp"


namespace swatch {
namespace mp7 {

//---
MP7RxPort::MP7RxPort( const std::string& aId, uint32_t aChannelID, ::mp7::MP7Controller& aController ) :
  InputPort(aId),
  mChannelID(aChannelID),
  mDriver(aController),
  mDatapath(mDriver.getDatapath()),
  mMgt(mDatapath.getNode< ::mp7::MGTRegionNode>("region.mgt")),
  mAlign(mDatapath.getNode< ::mp7::AlignMonNode>("region.align")),
  mMetricPacketCounter(registerMetric<uint32_t>("packetCounter")),
  mMetricAlignBx(registerMetric<uint32_t>("alignBx")),
  mMetricAlignCycle(registerMetric<uint32_t>("alignCycle")),
  mMetricAlignErrors(registerMetric<uint32_t>("alignErrors"))    
{
  
  setWarningCondition(mMetricPacketCounter,core::EqualCondition<uint32_t>(0));

  setWarningCondition(mMetricAlignBx,core::EqualCondition<uint32_t>(0xfff));

  setWarningCondition(mMetricAlignCycle,core::EqualCondition<uint32_t>(0x7));

  setErrorCondition(mMetricAlignErrors,core::NotEqualCondition<uint32_t>(0x0));

}


//---
MP7RxPort::~MP7RxPort() {
}


//---
void MP7RxPort::retrieveMetricValues()
{
  // Select the link, and calculate channel's local ID (within quad) ...
  mDatapath.selectLink(this->mChannelID);
  uint32_t localId = ::mp7::ChannelGroup::channelToLocal(mChannelID);
  

  /* IS LOCKED */
  // Point to the right node
  std::string mgtStatusNodePath = "ro_regs.ch" + boost::lexical_cast<std::string>(localId) + ".status";
  uhal::ValWord<uint32_t> mgtInReset = mMgt.getNode(mgtStatusNodePath+".rxusrrst").read();
  uhal::ValWord<uint32_t> mgtResetDone = mMgt.getNode(mgtStatusNodePath+".rx_fsm_reset_done").read();
  uhal::ValWord<uint32_t> mgtNoCRCs = mMgt.getNode(mgtStatusNodePath+".crc_checked").read();

  /* IS ALIGNED */
  ::mp7::AlignStatus alStatus = mAlign.readStatus();
//  uhal::ValWord<uint32_t> alignErrors = mAlign.getNode("stat.err_cnt").read();
  
  
  /* CRC ERRORS */
  //if (aChannel > 3)
  //  throw mp7:MGTChannelIdOutOfBounds("Invalid channel number requested");
  uhal::ValWord<uint32_t> crcErrors = mMgt.getNode(mgtStatusNodePath+".crc_error").read();
  mDriver.hw().dispatch();

  bool isLocked (
    // Not in reset
    !mgtInReset.value() and
    // Reset completed
    mgtResetDone.value() and
    // And no crcs
    mgtNoCRCs.value() != 0x0
  );

  setMetricValue<>(mMetricIsLocked, isLocked);

  setMetricValue<>(mMetricPacketCounter, mgtNoCRCs.value());

  setMetricValue<>(mMetricCRCErrors, crcErrors.value());
  
  setMetricValue<>(mMetricIsAligned, alStatus.marker and alStatus.errors == 0);
  
  setMetricValue<>(mMetricAlignBx, alStatus.position.bx);

  setMetricValue<>(mMetricAlignCycle, alStatus.position.cycle);

  setMetricValue<>(mMetricAlignErrors, alStatus.errors);

}



//---
MP7TxPort::MP7TxPort(const std::string& aId, uint32_t aChannelID, ::mp7::MP7Controller& aController) :
  OutputPort(aId),
  mChannelID(aChannelID),
  mDriver(aController),
  mDatapath(mDriver.getDatapath()),
  mMgt(mDatapath.getNode< ::mp7::MGTRegionNode>("region.mgt"))
{
}


//---
MP7TxPort::~MP7TxPort() {
}


//---
void MP7TxPort::retrieveMetricValues()
{  
  /* IS OPERATING */
  mDatapath.selectLink(mChannelID);
  
  // Calculate the channel local id
  uint32_t l = ::mp7::ChannelGroup::channelToLocal(mChannelID);

  // Point to the right node
  std::string path = ::mp7::strprintf("ro_regs.ch%d.status", l);

  // Get all subregisters
  ::mp7::Snapshot s =  ::mp7::snapshot(mMgt.getNode(path));

  bool isOperating (
    // Not in reset
    !s["txusrrst"] and
    // Reset completed
    s["tx_fsm_reset_done"]
  );

  setMetricValue<>(mMetricIsOperating, isOperating);
}

}
}
