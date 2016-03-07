/**
 * @file    MP7Processor.cpp
 * @author  Alessandro Thea
 * @brief   MP7 board processor implementation
 * @date    06/11/14
 */

#include "swatch/mp7/MP7Processor.hpp"


// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/processor/PortCollection.hpp"
#include "swatch/processor/ProcessorStub.hpp"

// SWATCH MP7 headers
#include "swatch/mp7/BufferCommands.hpp"
#include "swatch/mp7/FormatterCommands.hpp"
#include "swatch/mp7/MGTCommands.hpp"
#include "swatch/mp7/MmcCommands.hpp"
#include "swatch/mp7/GeneralCommands.hpp"
#include "swatch/mp7/MP7Ports.hpp"
#include "swatch/mp7/MP7ReadoutInterface.hpp"
#include "swatch/mp7/MP7TTCInterface.hpp"
#include "swatch/mp7/ReadoutCommands.hpp"


// MP7 headers
#include "mp7/MP7Controller.hpp"

// uHAL headers
#include "uhal/HwInterface.hpp"
#include "uhal/ConnectionManager.hpp"

// log4cplus headers
#include "log4cplus/loggingmacros.h"

// C++ headers
#include <iomanip>


namespace swpro = swatch::processor;


SWATCH_REGISTER_CLASS(swatch::mp7::MP7Processor);


namespace swatch {
namespace mp7 {

// Static Members Initialization
const std::string MP7Processor::CmdIds::kUploadFw = "uploadFw";
const std::string MP7Processor::CmdIds::kDeleteFw = "deleteFw";
const std::string MP7Processor::CmdIds::kReboot = "reboot";
const std::string MP7Processor::CmdIds::kHardReset = "hardReset";
const std::string MP7Processor::CmdIds::kScanSD = "scanSD";
const std::string MP7Processor::CmdIds::kReset = "resetBoard";
const std::string MP7Processor::CmdIds::kZeroInputs = "zeroInputs";
const std::string MP7Processor::CmdIds::kSetId = "setId";
const std::string MP7Processor::CmdIds::kCfgRxMGTs = "cfgRxMGTs";
const std::string MP7Processor::CmdIds::kCfgTxMGTs = "cfgTxMGTs";
const std::string MP7Processor::CmdIds::kAlignMGTs = "alignMGTs";
const std::string MP7Processor::CmdIds::kAutoAlignMGTs = "autoAlignMGTs";
const std::string MP7Processor::CmdIds::kCfgRxBuffers = "cfgRxBuffers";
const std::string MP7Processor::CmdIds::kCfgTxBuffers = "cfgTxBuffers";
const std::string MP7Processor::CmdIds::kCaptureBuffers = "capture";
const std::string MP7Processor::CmdIds::kSaveRxBuffers = "saveRxBuffers";
const std::string MP7Processor::CmdIds::kSaveTxBuffers = "saveTxBuffers";
const std::string MP7Processor::CmdIds::kCfgLatencyRxBuffers = "latencyRxBuffers";
const std::string MP7Processor::CmdIds::kCfgLatencyTxBuffers = "latencyTxBuffers";
const std::string MP7Processor::CmdIds::kCfgEasyRxLatency = "easyRxLatency";
const std::string MP7Processor::CmdIds::kCfgEasyTxLatency = "easyTxLatency";
const std::string MP7Processor::CmdIds::kCfgFormatterTdr = "cfgFormatterTdr";
const std::string MP7Processor::CmdIds::kSetupReadout = "roSetup";
const std::string MP7Processor::CmdIds::kLoadReadoutMenu = "roLoadMenu";

//---
MP7Processor::MP7Processor(const swatch::core::AbstractStub& aStub) :
MP7AbstractProcessor(aStub)
{
  // Extract stub, and create driver
  const processor::ProcessorStub& stub = getStub();

  mDriver = new ::mp7::MP7Controller(uhal::ConnectionManager::getDevice(stub.id, stub.uri, stub.addressTable));

  // Build subcomponents
  registerInterface(new MP7TTCInterface(*mDriver));
  registerInterface(new MP7ReadoutInterface(*mDriver));
  registerInterface(new swpro::InputPortCollection());
  registerInterface(new swpro::OutputPortCollection());

  buildPorts(stub);

  // Register default MP7 commands
  registerCommand<UploadFirmwareCommand>(CmdIds::kUploadFw);
  registerCommand<DeleteFirmwareCommand>(CmdIds::kDeleteFw);
  registerCommand<RebootFPGACommand>(CmdIds::kReboot);
  registerCommand<HardResetCommand>(CmdIds::kHardReset);
  registerCommand<ScanSDCommand>(CmdIds::kScanSD);
  registerCommand<ResetCommand>(CmdIds::kReset);
  registerCommand<ZeroInputsCommand>(CmdIds::kZeroInputs);
  registerCommand<SetIDCommand>(CmdIds::kSetId);
  registerCommand<ConfigureRxMGTsCommand>(CmdIds::kCfgRxMGTs);
  registerCommand<ConfigureTxMGTsCommand>(CmdIds::kCfgTxMGTs);
  registerCommand<AlignRxsToCommand>(CmdIds::kAlignMGTs);
  registerCommand<AutoAlignCommand>(CmdIds::kAutoAlignMGTs);
  registerCommand<ConfigureRxBuffersCommand>(CmdIds::kCfgRxBuffers);
  registerCommand<ConfigureTxBuffersCommand>(CmdIds::kCfgTxBuffers);
  registerCommand<CaptureBuffersCommand>(CmdIds::kCaptureBuffers);
  registerCommand<SaveRxBuffersToFileCommand>(CmdIds::kSaveRxBuffers);
  registerCommand<SaveTxBuffersToFileCommand>(CmdIds::kSaveTxBuffers);
  registerCommand<LatencyRxBuffersCommand>(CmdIds::kCfgLatencyRxBuffers);
  registerCommand<LatencyTxBuffersCommand>(CmdIds::kCfgLatencyTxBuffers);
  registerCommand<EasyRxLatencyCommand>(CmdIds::kCfgEasyRxLatency);
  registerCommand<EasyTxLatencyCommand>(CmdIds::kCfgEasyTxLatency);
  registerCommand<TDRFormatterCommand>(CmdIds::kCfgFormatterTdr);
  registerCommand<SetupReadout>(CmdIds::kSetupReadout);
  registerCommand<LoadReadoutMenu>(CmdIds::kLoadReadoutMenu);

  uint64_t lFwVsn = retrieveFirmwareVersion();
  LOG4CPLUS_INFO(getLogger(), "MP7 processor '" << this->getId() << "' built: firmware 0x" << std::hex << lFwVsn);
}

//---
MP7Processor::~MP7Processor()
{
  delete mDriver;
}


//---
void MP7Processor::buildPorts(const processor::ProcessorStub& stub)
{

  // 
  // HACK ALERT
  // 
  const std::vector<uint32_t>& lMGTIDs = mDriver->getChannelIDs(::mp7::kLinkIDs).channels();
  const std::vector<uint32_t>& lBufferIDs = mDriver->getChannelIDs(::mp7::kBufferIDs).channels();
  const std::vector<uint32_t>& lTDRFmtIDs = mDriver->getChannelIDs(::mp7::kTDRFmtIDs).channels();
  const std::vector<uint32_t>& lDmxFmtIDs = mDriver->getChannelIDs(::mp7::kDemuxFmtIDs).channels();

  // Add input ports
  for (auto it = stub.rxPorts.begin(); it != stub.rxPorts.end(); it++) {

    MP7RxPort* rxPort = new MP7RxPort(it->id, it->number, *mDriver);
    getInputPorts().addPort(rxPort);

    // Fetch mgt status
    bool lHasMGT = (std::find(lMGTIDs.begin(), lMGTIDs.end(), it->number) != lMGTIDs.end());

    // Fetch buffer status
    bool lHasBuffer = (std::find(lBufferIDs.begin(), lBufferIDs.end(), it->number) != lBufferIDs.end());

    // Resolve formatter kind
    ::mp7::FormatterKind lFmtKind = ::mp7::kEmptyFormatter;
    // Assign TDRFormatter fmt kind if port number is in the TDRFormatter group
    if (std::find(lTDRFmtIDs.begin(), lTDRFmtIDs.end(), it->number) != lTDRFmtIDs.end()) {
      lFmtKind = ::mp7::kTDRFormatter;
    }
    // Assign DemuxFormatter fmt kind if port number is in the DemuxFormatter group
    else if (std::find(lDmxFmtIDs.begin(), lDmxFmtIDs.end(), it->number) != lDmxFmtIDs.end()) {
      lFmtKind = ::mp7::kDemuxFormatter;
    }

    mRxDescriptors[it->id] = channel::Descriptor(it->number, lHasMGT, lHasBuffer, lFmtKind, rxPort);

  }


  for (auto it = stub.txPorts.begin(); it != stub.txPorts.end(); it++) {

    getOutputPorts().addPort(new MP7TxPort(it->id, it->number, *mDriver));

    // Fetch mgt status
    bool lHasMGT = (std::find(lMGTIDs.begin(), lMGTIDs.end(), it->number) != lMGTIDs.end());

    // Fetch buffer status
    bool lHasBuffer = (std::find(lBufferIDs.begin(), lBufferIDs.end(), it->number) != lBufferIDs.end());

    // Resolve formatter kind
    ::mp7::FormatterKind lFmtKind = ::mp7::kEmptyFormatter;
    // Assign TDRFormatter fmt kind if port number is in the TDRFormatter group
    if (std::find(lTDRFmtIDs.begin(), lTDRFmtIDs.end(), it->number) != lTDRFmtIDs.end()) {
      lFmtKind = ::mp7::kTDRFormatter;
    }
      // Assign DemuxFormatter fmt kind if port number is in the DemuxFormatter group
    else if (std::find(lDmxFmtIDs.begin(), lDmxFmtIDs.end(), it->number) != lDmxFmtIDs.end()) {
      lFmtKind = ::mp7::kDemuxFormatter;
    }

    mTxDescriptors[it->id] = channel::Descriptor(it->number, lHasMGT, lHasBuffer, lFmtKind);

  }
}


::mp7::MP7Controller& MP7Processor::driver()
{
  return *mDriver;
}


uint64_t
MP7Processor::retrieveFirmwareVersion() const
{
  uhal::ValWord<uint32_t> v = mDriver->getCtrl().getNode("id.fwrev").read();
  mDriver->hw().dispatch();

  return v.value();
}


std::string
MP7Processor::firmwareInfo() const
{
  return "";
}


void MP7Processor::retrieveMetricValues()
{

  setMetricValue<>(metricFirmwareVersion_, retrieveFirmwareVersion());

}

} // namespace mp7
} // namespace swatch