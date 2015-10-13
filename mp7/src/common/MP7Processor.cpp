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


MP7Processor::MP7Processor(const swatch::core::AbstractStub& aStub) :
  Processor(aStub),
  mDriver( new ::mp7::MP7Controller(uhal::ConnectionManager::getDevice(getStub().id, getStub().uri, getStub().addressTable)) ),
  mUploadFw( registerFunctionoid<UploadFirmwareCommand>("uploadFw") ),
  mDeleteFw( registerFunctionoid<DeleteFirmwareCommand>("deleteFw") ),
  mReboot( registerFunctionoid<RebootFPGACommand>("reboot") ),
  mHardReset( registerFunctionoid<HardResetCommand>("hardReset") ),
  mScanSD( registerFunctionoid<ScanSDCommand>("scanSD") ),
  mReset( registerFunctionoid<ResetCommand>("resetBoard") ),
  mCfgRxMGTs( registerFunctionoid<ConfigureRxMGTsCommand>("cfgRxMGTs") ),
  mCfgTxMGTs( registerFunctionoid<ConfigureTxMGTsCommand>("cfgTxMGTs") ),
  mAlignMGTs( registerFunctionoid<AlignRxsToCommand>("alignMGTs") ),
  mAutoAlignMGTs( registerFunctionoid<AutoAlignCommand>("autoAlignMGTs") ),
  mCfgRxBuffers( registerFunctionoid<ConfigureRxBuffersCommand>("cfgRxBuffers") ),
  mCfgTxBuffers( registerFunctionoid<ConfigureTxBuffersCommand>("cfgTxBuffers") ),
  mCaptureBuffers( registerFunctionoid<CaptureBuffersCommand>("capture") ),
  mSaveRxBuffers( registerFunctionoid<SaveRxBuffersToFileCommand>("saveRxBuffers") ),
  mSaveTxBuffers( registerFunctionoid<SaveTxBuffersToFileCommand>("saveTxBuffers") ),
  mCfgLatencyRxBuffers( registerFunctionoid<LatencyRxBuffersCommand>("latencyRxBuffers") ),
  mCfgLatencyTxBuffers( registerFunctionoid<LatencyTxBuffersCommand>("latencyTxBuffers") ),
  mCfgEasyRxLatency( registerFunctionoid<EasyRxLatencyCommand>("easyRxLatency") ),
  mCfgEasyTxLatency( registerFunctionoid<EasyTxLatencyCommand>("easyTxLatency") ),
  mCfgFormatterTdr( registerFunctionoid<TDRFormatterCommand>("cfgFormatterTdr")),
  mSetupReadout( registerFunctionoid<SetupReadout>("roSetup") ),
  mLoadReadoutMenu( registerFunctionoid("roLoadMenu", createLoadReadoutMenuCommand("roLoadMenu", *mDriver)) )
{
  // Extract stub, and create driver
  const processor::ProcessorStub& stub = getStub();

  // Build subcomponents
  registerInterface( new MP7TTCInterface( *mDriver ) ); 
  registerInterface( new MP7ReadoutInterface( *mDriver) );
  registerInterface( new swpro::PortCollection() );

  // Add input and output ports
  for(auto it = stub.rxPorts.begin(); it != stub.rxPorts.end(); it++)
    getPorts().addInput(new MP7RxPort(it->id, it->number, *this));
  for(auto it = stub.txPorts.begin(); it != stub.txPorts.end(); it++)
    getPorts().addOutput(new MP7TxPort(it->id, it->number, *this));

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

core::Command* MP7Processor::createLoadReadoutMenuCommand(const std::string& aId, const ::mp7::MP7Controller& aController) {
  const ::mp7::ReadoutCtrlNode& rc = aController.getReadout().getNode< ::mp7::ReadoutCtrlNode >("readout_control");
  uint32_t lBanks = rc.readNumBanks();
  uint32_t lModes = rc.readNumModes();
  uint32_t lCaptures = rc.readNumCaptures();

  return new LoadReadoutMenu(aId, lBanks, lModes, lCaptures);
}



} // namespace mp7
} // namespace swatch
