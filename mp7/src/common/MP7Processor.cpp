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
#include "swatch/mp7/ResetCommand.hpp"
#include "swatch/mp7/MP7Ports.hpp"
#include "swatch/mp7/MP7ReadoutInterface.hpp"
#include "swatch/mp7/MP7TTCInterface.hpp"
#include "swatch/mp7/ReadoutCommands.hpp"


// MP7 headers
#include "mp7/MP7Controller.hpp"

// uHAL headers
#include "uhal/HwInterface.hpp"
#include "uhal/ConnectionManager.hpp"

// C++ Headers
#include <iomanip>


namespace swlog = swatch::logger;
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

MP7Processor::MP7Processor(const swatch::core::AbstractStub& aStub) :
  Processor(aStub)
//  mUploadFw( registerFunctionoid<UploadFirmwareCommand>("uploadFw") ),
//  mDeleteFw( registerFunctionoid<DeleteFirmwareCommand>("deleteFw") ),
//  mReboot( registerFunctionoid<RebootFPGACommand>("reboot") ),
//  mHardReset( registerFunctionoid<HardResetCommand>("hardReset") ),
//  mScanSD( registerFunctionoid<ScanSDCommand>("scanSD") ),
//  mReset( registerFunctionoid<ResetCommand>("resetBoard") ),
//  mCfgRxMGTs( registerFunctionoid<ConfigureRxMGTsCommand>("cfgRxMGTs") ),
//  mCfgTxMGTs( registerFunctionoid<ConfigureTxMGTsCommand>("cfgTxMGTs") ),
//  mAlignMGTs( registerFunctionoid<AlignRxsToCommand>("alignMGTs") ),
//  mAutoAlignMGTs( registerFunctionoid<AutoAlignCommand>("autoAlignMGTs") ),
//  mCfgRxBuffers( registerFunctionoid<ConfigureRxBuffersCommand>("cfgRxBuffers") ),
//  mCfgTxBuffers( registerFunctionoid<ConfigureTxBuffersCommand>("cfgTxBuffers") ),
//  mCaptureBuffers( registerFunctionoid<CaptureBuffersCommand>("capture") ),
//  mSaveRxBuffers( registerFunctionoid<SaveRxBuffersToFileCommand>("saveRxBuffers") ),
//  mSaveTxBuffers( registerFunctionoid<SaveTxBuffersToFileCommand>("saveTxBuffers") ),
//  mCfgLatencyRxBuffers( registerFunctionoid<LatencyRxBuffersCommand>("latencyRxBuffers") ),
//  mCfgLatencyTxBuffers( registerFunctionoid<LatencyTxBuffersCommand>("latencyTxBuffers") ),
//  mCfgEasyRxLatency( registerFunctionoid<EasyRxLatencyCommand>("easyRxLatency") ),
//  mCfgEasyTxLatency( registerFunctionoid<EasyTxLatencyCommand>("easyTxLatency") ),
//  mCfgFormatterTdr( registerFunctionoid<TDRFormatterCommand>("cfgFormatterTdr")),
//  mSetupReadout( registerFunctionoid<SetupReadout>("roSetup") ),
//  mLoadReadoutMenu( registerFunctionoid("roLoadMenu", LoadReadoutMenu::create("roLoadMenu", *mDriver)) )
{
  // Extract stub, and create driver
  const processor::ProcessorStub& stub = getStub();

  mDriver = new ::mp7::MP7Controller(uhal::ConnectionManager::getDevice(stub.id, stub.uri, stub.addressTable));

  // Build subcomponents
  registerInterface( new MP7TTCInterface( *mDriver ) ); 
  registerInterface( new MP7ReadoutInterface( *mDriver) );
  registerInterface( new swpro::PortCollection() );

  // Add input and output ports
  for(auto it = stub.rxPorts.begin(); it != stub.rxPorts.end(); it++)
    getPorts().addInput(new MP7RxPort(it->id, it->number, *this));
  for(auto it = stub.txPorts.begin(); it != stub.txPorts.end(); it++)
    getPorts().addOutput(new MP7TxPort(it->id, it->number, *this));

  // Register default MP7 commands
  registerCommand<UploadFirmwareCommand>(CmdIds::kUploadFw);
  registerCommand<DeleteFirmwareCommand>(CmdIds::kDeleteFw);
  registerCommand<RebootFPGACommand>(CmdIds::kReboot);
  registerCommand<HardResetCommand>(CmdIds::kHardReset);
  registerCommand<ScanSDCommand>(CmdIds::kScanSD);
  registerCommand<ResetCommand>(CmdIds::kReset);
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
  registerCommand(CmdIds::kLoadReadoutMenu, LoadReadoutMenu::create("roLoadMenu", *mDriver));

  LOG(swlog::kNotice) << "MP7 Processor '" << this->getId() << "' built: firmware 0x" << std::hex << retrieveFirmwareVersion() << std::endl;
}

MP7Processor::~MP7Processor() {
  delete mDriver;
}


uint64_t
MP7Processor::retrieveFirmwareVersion() const {
    uhal::ValWord<uint32_t> v = mDriver->getCtrl().getNode("id.fwrev").read();
    mDriver->hw().dispatch();
    
    return v.value();
}

std::string
MP7Processor::firmwareInfo() const {
  return "";
}

void MP7Processor::retrieveMetricValues() {
  
  setMetricValue<>(metricFirmwareVersion_, retrieveFirmwareVersion());

}
//
//core::Command* MP7Processor::createLoadReadoutMenuCommand(const std::string& aId, const ::mp7::MP7Controller& aController) {
//  const ::mp7::ReadoutCtrlNode& rc = aController.getReadout().getNode< ::mp7::ReadoutCtrlNode >("readout_control");
//  uint32_t lBanks = rc.readNumBanks();
//  uint32_t lModes = rc.readNumModes();
//  uint32_t lCaptures = rc.readNumCaptures();
//
//  return new LoadReadoutMenu(aId, lBanks, lModes, lCaptures);
//}



} // namespace mp7
} // namespace swatch
