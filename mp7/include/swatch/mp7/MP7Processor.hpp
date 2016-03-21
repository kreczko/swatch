#ifndef SWATCH_MP7_MP7PROCESSOR_HPP
#define SWATCH_MP7_MP7PROCESSOR_HPP


#include "swatch/mp7/MP7AbstractProcessor.hpp"
#include "swatch/mp7/channel/Descriptor.hpp"
#include "mp7/MP7Controller.hpp"

namespace swatch {
namespace mp7 {

// Temporary class
struct MP7ClockMode {
  std::string clkCfg;
  std::string clk40Src;
  std::string ttcCfg;
};

class MP7Processor : public MP7AbstractProcessor {
public:
  MP7Processor(const swatch::core::AbstractStub& aStub);
  virtual ~MP7Processor();

  virtual uint64_t retrieveFirmwareVersion() const;

  virtual std::string firmwareInfo() const;

  virtual ::mp7::MP7Controller& driver();
 

private:
  ::mp7::MP7Controller* mDriver;

  void buildPorts( const processor::ProcessorStub& aStub );


protected:
  virtual void retrieveMetricValues();

  static const std::string lUploadFwCmdId;

  struct CmdIds {
    static const std::string kUploadFw;
    static const std::string kDeleteFw;
    static const std::string kReboot;
    static const std::string kHardReset;
    static const std::string kScanSD;
    static const std::string kReset;
    static const std::string kZeroInputs;
    static const std::string kSetId;
    static const std::string kCfgRxMGTs;
    static const std::string kCfgTxMGTs;
    static const std::string kAlignMGTs;
    static const std::string kAutoAlignMGTs;
    static const std::string kCfgRxBuffers;
    static const std::string kCfgTxBuffers;
    static const std::string kCaptureBuffers;
    static const std::string kSaveRxBuffers;
    static const std::string kSaveTxBuffers;
    static const std::string kCfgLatencyRxBuffers;
    static const std::string kCfgLatencyTxBuffers;
    static const std::string kCfgEasyRxLatency;
    static const std::string kCfgEasyTxLatency;
    static const std::string kHdrFormatter;
    static const std::string kDataValidFmt;
    static const std::string kSetupReadout;
    static const std::string kLoadReadoutMenu;
  };




private:
  //  static core::Command* createLoadReadoutMenuCommand(const std::string& aId, const ::mp7::MP7Controller& aController);    
};

} // namespace mp7 
} // namespace swatch


#endif

