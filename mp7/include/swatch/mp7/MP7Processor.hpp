#ifndef SWATCH_MP7_MP7PROCESSOR_HPP
#define SWATCH_MP7_MP7PROCESSOR_HPP


#include "swatch/processor/Processor.hpp"


// Forward declarations
namespace mp7 {
class MP7Controller;
}


namespace swatch {
namespace mp7 {

// Temporary class
struct MP7ClockMode {
    std::string clkCfg;
    std::string clk40Src;
    std::string ttcCfg;
};

class MP7Processor: public processor::Processor {
public:
    MP7Processor(const swatch::core::AbstractStub& aStub);
    virtual ~MP7Processor();
    
    virtual uint64_t retrieveFirmwareVersion() const;
    
    virtual std::string firmwareInfo() const;

    ::mp7::MP7Controller& driver() { return *mDriver; } 

private:
    ::mp7::MP7Controller* mDriver;

protected:
    virtual void retrieveMetricValues();
    
    core::Command& mUploadFw;
    core::Command& mDeleteFw;
    core::Command& mReboot;
    core::Command& mHardReset;
    core::Command& mScanSD;
    core::Command& mReset;
    core::Command& mCfgRxMGTs;
    core::Command& mCfgTxMGTs;
    core::Command& mAlignMGTs;
    core::Command& mAutoAlignMGTs;
    core::Command& mCfgRxBuffers;
    core::Command& mCfgTxBuffers;
    core::Command& mCaptureBuffers;
    core::Command& mSaveRxBuffers;
    core::Command& mSaveTxBuffers;
    core::Command& mCfgLatencyRxBuffers;
    core::Command& mCfgLatencyTxBuffers;
    core::Command& mCfgEasyRxLatency;
    core::Command& mCfgEasyTxLatency;
    core::Command& mCfgFormatterTdr;
    core::Command& mSetupReadout;
    core::Command& mLoadReadoutMenu;
    
private:
  static core::Command* createLoadReadoutMenuCommand(const std::string& aId, const ::mp7::MP7Controller& aController);    
};

} // namespace mp7 
} // namespace swatch


#endif

