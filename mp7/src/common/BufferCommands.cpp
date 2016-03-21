
#include "swatch/mp7/BufferCommands.hpp"


// STL headers
#include <sstream>
#include <iomanip>

// boost headers
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <log4cplus/loggingmacros.h>

// XDAQ headers
#include "xdata/String.h"
#include "xdata/UnsignedInteger.h"

// MP7 Core Headers
#include "mp7/BoardData.hpp"
#include "mp7/MP7Controller.hpp"
#include "mp7/PathConfigurator.hpp"
#include "mp7/TTCNode.hpp"
#include "mp7/operators.hpp"
#include "mp7/Logger.hpp"

// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/mp7/MP7Processor.hpp"
#include "swatch/mp7/Utilities.hpp"


namespace swatch {
namespace mp7 {


ZeroInputsCommand::ZeroInputsCommand(const std::string& aId, swatch::core::ActionableObject& aActionable) :
swatch::core::Command(aId, aActionable, xdata::String())
{

}


swatch::core::Command::State ZeroInputsCommand::code(const ::swatch::core::XParameterSet& params)
{
  // Extract the MP7 driver
  ::mp7::MP7MiniController& driver = getActionable<MP7AbstractProcessor>().driver();
  ::mp7::orbit::Metric metric = driver.getMetric();
  
  // Create a base configurator
  ::mp7::TestPathConfigurator lZeroConfigurator = ::mp7::TestPathConfigurator(::mp7::TestPathConfigurator::kZeroes, ::mp7::orbit::Point(), metric);

  // Configure 
  ::mp7::ChannelManager cm = driver.channelMgr();

  // 
  std::ostringstream zeroedIds;
  zeroedIds << ::mp7::logger::shortVecFmt(cm.getDescriptor().pickRxBufferIDs(::mp7::kBuffer).channels());
  setStatusMsg("Masking MP7 inputs " + zeroedIds.str());

  // Zero
  cm.configureBuffers(::mp7::kRx, lZeroConfigurator);
  
  setResult(xdata::String("Masked MP7 inputs "+zeroedIds.str()));

  return State::kDone;
}


// Static initialization
template<class Selector>
const std::map< std::string, ::mp7::TestPathConfigurator::Mode > ConfigureBuffersCommand<Selector>::mBufferModeMap = {
  {"Latency", ::mp7::TestPathConfigurator::kLatency},
  {"Capture", ::mp7::TestPathConfigurator::kCapture},
  {"PlayOnce", ::mp7::TestPathConfigurator::kPlayOnce},
  {"PlayLoop", ::mp7::TestPathConfigurator::kPlayLoop},
  {"Pattern", ::mp7::TestPathConfigurator::kPattern},
  {"Zeroes", ::mp7::TestPathConfigurator::kZeroes},
  {"CaptureStrobe", ::mp7::TestPathConfigurator::kCaptureStrobe},
  {"Pattern3G", ::mp7::TestPathConfigurator::kPattern3G},
  {"PlayOnceStrobe", ::mp7::TestPathConfigurator::kPlayOnceStrobe},
  {"PlayOnce3G", ::mp7::TestPathConfigurator::kPlayOnce3G}
};

// // template specialisation
// template<>
// const ::mp7::bufferkind buffertraits<abstractchannelscommand::krx>::bufferkind = ::mp7::krxbuffer;

// template<>
// const ::mp7::bufferkind buffertraits<abstractchannelscommand::ktx>::bufferkind = ::mp7::ktxbuffer;

// Template specialisation
template<>
const ::mp7::RxTxSelector BufferTraits<RxBufferSelector>::kRxTxSelector = ::mp7::kRx;

template<>
const ::mp7::RxTxSelector BufferTraits<TxBufferSelector>::kRxTxSelector = ::mp7::kTx;

// --------------------------------------------------------
template<class Selector>
ConfigureBuffersCommand<Selector>::ConfigureBuffersCommand(const std::string& aId, swatch::core::ActionableObject& aActionable) :
swatch::core::Command(aId, aActionable, xdata::String()),
mBufferSelector(*this)
{

  mBufferSelector.addCommandParameters();

  // Parameter registration  
  registerParameter("startBx", xdata::UnsignedInteger(0x0));
  registerParameter("startCycle", xdata::UnsignedInteger(0x0));
  registerParameter("stopBx", xdata::UnsignedInteger());
  registerParameter("stopCycle", xdata::UnsignedInteger());
  registerParameter("payload", xdata::String(""));
  registerParameter("mode", xdata::String(""));
}



// --------------------------------------------------------
template<class Selector>
core::Command::State
ConfigureBuffersCommand<Selector>::code(const ::swatch::core::XParameterSet& params)
{

  const ::mp7::RxTxSelector bKind = BufferTraits<Selector>::kRxTxSelector;

  // Extract parameter values
  std::string modeValue = params.get<xdata::String>("mode").value_;

  uint startBxVal = params.get<xdata::UnsignedInteger>("startBx").value_;
  uint startCycleVal = params.get<xdata::UnsignedInteger>("startCycle").value_;

  uint stopBxVal = params.get<xdata::UnsignedInteger>("stopBx").value_;
  uint stopCycleVal = params.get<xdata::UnsignedInteger>("stopCycle").value_;

  std::string payload = params.get<xdata::String>("payload").value_;

  const xdata::UnsignedInteger& startBx = params.get<xdata::UnsignedInteger>("startBx");
  const xdata::UnsignedInteger& startCycle = params.get<xdata::UnsignedInteger>("startCycle");
  const xdata::UnsignedInteger& stopBx = params.get<xdata::UnsignedInteger>("stopBx");
  const xdata::UnsignedInteger& stopCycle = params.get<xdata::UnsignedInteger>("stopCycle");

  // Instantiate the message streamer once
  std::ostringstream msg;
  // Extract the MP7 driver
  ::mp7::MP7MiniController& driver = getActionable<MP7AbstractProcessor>().driver();
  ::mp7::orbit::Metric metric = driver.getMetric();

  // TOFIX: make check standard
  if (!orbit::isValid(startBx, startCycle, metric)) {
    std::ostringstream msg;
    msg << "Invalid orbit point parameters (" << startBx.value_ << ", " << startCycle.value_ << ")";

    setStatusMsg(msg.str());
    return State::kError;
  }


  size_t bufferSize = driver.getBuffer().getBufferSize();
  LOG(swatch::logger::kWarning) << "bufferSize " << " " << bufferSize;

  msg.str("");
  msg << "Configuring " << bKind << " buffer...";
  setProgress(0., msg.str());

  // Reset msg
  msg.str("");
  // Populate message
  msg << "Mode set: " << modeValue;

  if (!payload.empty())
    msg << " reading from: " << payload;

  setStatusMsg(msg.str());

  ::mp7::ChannelManager cm = mBufferSelector.manager(params);

  setProgress(0.2, "Generating BoardData object...");

  // Reset msg
  msg.str("");
  // Populate message
  msg << "Setting range : start point " << startBx.value_ << ", " << startCycle.value_;
  setProgress(0.4, msg.str());

  ::mp7::TestPathConfigurator::Mode lBufMode;
  try {
    lBufMode = mBufferModeMap.at(modeValue);
  } catch (std::out_of_range& e) {
    std::ostringstream msg;
    msg << "Unknown buffer mode: '" << modeValue << "'";
    setStatusMsg(msg.str());
    return State::kError;
  }

  ::mp7::orbit::Point startPoint = ::mp7::orbit::Point(startBxVal, startCycleVal);

  // Create a base configurator
  ::mp7::TestPathConfigurator pc = ::mp7::TestPathConfigurator(lBufMode, startPoint, metric);

  // If stop is defined, update it
  if (!orbit::isNull(stopBx, stopCycle)) {

    // Ensure the end orbit point is good
    if (!orbit::isValid(stopBx, stopCycle, metric)) {
      std::ostringstream msg;
      msg << "Invalid orbit point parameters (" << stopBx.value_ << ", " << stopCycle.value_ << ")";

      setStatusMsg(msg.str());
      return State::kError;
    }

    msg << "Setting range : stop point " << stopBx.value_ << ", " << stopCycle.value_;

    ::mp7::orbit::Point stopPoint = ::mp7::orbit::Point(stopBxVal, stopCycleVal);

    // Update configurator with fixed range
    ::mp7::TestPathConfigurator pc = ::mp7::TestPathConfigurator(lBufMode, startPoint, stopPoint, metric);

  }

  setProgress(0.6, "Configuring buffers...");


  // Apply configuration
  cm.configureBuffers(bKind, pc);

  if (!payload.empty()) {
    LOG(swatch::logger::kWarning) << "loading " << payload;

    ::mp7::BoardData data = ::mp7::BoardDataFactory::generate(payload, bufferSize, true);
    cm.clearBuffers(bKind);
    cm.loadPatterns(bKind, data);
  } else {
    cm.clearBuffers(bKind, ::mp7::ChanBufferNode::kCapture);
  }

  setStatusMsg("Configure Buffers completed");

  return State::kDone;
}


template class ConfigureBuffersCommand<RxBufferSelector>;
template class ConfigureBuffersCommand<TxBufferSelector>;



// --------------------------------------------------------
CaptureBuffersCommand::CaptureBuffersCommand(const std::string& aId, swatch::core::ActionableObject& aActionable) :
swatch::core::Command(aId, aActionable, xdata::String())
{
}


// --------------------------------------------------------
core::Command::State
CaptureBuffersCommand::code(const ::swatch::core::XParameterSet& params)
{

  ::mp7::MP7MiniController& driver = getActionable< MP7AbstractProcessor>().driver();

  ::mp7::TTCNode ttc = driver.getTTC();
  ::mp7::ChannelManager cm = driver.channelMgr();


  setProgress(0.5, "Clearing capture buffers ...");

  cm.clearBuffers(::mp7::kRx, ::mp7::ChanBufferNode::kCapture);
  cm.clearBuffers(::mp7::kTx, ::mp7::ChanBufferNode::kCapture);

  setProgress(0.5, "Capturing data stream ...");

  ttc.forceBTest();
  cm.waitCaptureDone();

  setStatusMsg("Capture completed");
  return State::kDone;
}



// --------------------------------------------------------
template<class Selector>
SaveBuffersToFileCommand<Selector>::SaveBuffersToFileCommand(const std::string& aId, swatch::core::ActionableObject& aActionable) :
swatch::core::Command(aId, aActionable, xdata::String()),
mBufferSelector(*this)
{

  mBufferSelector.addCommandParameters();

  registerParameter("filename", xdata::String(""));
}



// --------------------------------------------------------
template<class Selector>
core::Command::State SaveBuffersToFileCommand<Selector>::code(const ::swatch::core::XParameterSet& params)
{

  // Get the corresponding buffer kind
  const ::mp7::RxTxSelector bKind = BufferTraits<Selector>::kRxTxSelector;

  std::string filename = params.get<xdata::String>("filename").value_;

  //MP7AbstractProcessor* proc = getParent< MP7AbstractProcessor>();
  ::mp7::MP7MiniController& driver = getActionable< MP7AbstractProcessor>().driver();

  ::mp7::CtrlNode ctrl = driver.getCtrl();
  ::mp7::ChannelManager cm = mBufferSelector.manager(params);


  // TOFIX: Output file should be compulsory
  if (filename == "") {
    std::ostringstream fn;
    fn << bKind << "_summary.txt";
    filename = fn.str();
  } else {
    // Expand variables contained in the filename
    filename = swatch::core::shellExpandPath(filename);

    boost::filesystem::path p(filename);
    if (!boost::filesystem::exists(p.parent_path())) {
      try {
        boost::filesystem::create_directories(p.parent_path());
      } catch (std::exception& e) {
        std::ostringstream err;
        err << "Exception caught. Cannot create dir " << p.parent_path() << ". Please provide a valid path for captures.";
        setStatusMsg(err.str());
        return State::kError;
      }
    }
  }

  setProgress(0.1, "Reading buffers...");

  ::mp7::BoardData data = cm.readBuffers(bKind);

  setProgress(0.5, "Saving data...");

  ::mp7::BoardDataFactory::saveToFile(data, filename);

  setStatusMsg("Capture completed");
  return State::kDone;
}

// Template Instance
template class SaveBuffersToFileCommand<RxBufferSelector>;
template class SaveBuffersToFileCommand<TxBufferSelector>;



// --------------------------------------------------------
template<class Selector>
LatencyBuffersCommand<Selector>::LatencyBuffersCommand(const std::string& aId, swatch::core::ActionableObject& aActionable) :
swatch::core::Command(aId, aActionable, xdata::String()),
mBufferSelector(*this)
{

  mBufferSelector.addCommandParameters();

  registerParameter("bankId", xdata::UnsignedInteger(0x0));
  registerParameter("depth", xdata::UnsignedInteger(0x0));
}



// --------------------------------------------------------
template<class Selector>
core::Command::State LatencyBuffersCommand<Selector>::code(const ::swatch::core::XParameterSet& params)
{
  // Get the corresponding buffer kind
  const ::mp7::RxTxSelector bKind = BufferTraits<Selector>::kRxTxSelector;


  uint bankId = params.get<xdata::UnsignedInteger>("bankId").value_;
  uint depth = params.get<xdata::UnsignedInteger>("depth").value_;

  setProgress(0.0, "Configuring buffers in latency mode");

  ::mp7::ChannelManager cm = mBufferSelector.manager(params);
  ::mp7::LatencyPathConfigurator pc = ::mp7::LatencyPathConfigurator(bankId, depth);

  cm.configureBuffers(bKind, pc);

  setStatusMsg("Finished configuring buffers in latency mode");

  return State::kDone;
}

template class LatencyBuffersCommand<RxBufferSelector>;
template class LatencyBuffersCommand<TxBufferSelector>;


// --------------------------------------------------------
template<class Selector>
EasyLatencyCommand<Selector>::EasyLatencyCommand(const std::string& aId, swatch::core::ActionableObject& aActionable) :
swatch::core::Command(aId, aActionable, xdata::String()),
mBufferSelector(*this)
{

  mBufferSelector.addCommandParameters();

  registerParameter("bankId", xdata::UnsignedInteger(0x0));
  registerParameter("masterLatency", xdata::UnsignedInteger(0x0));
  registerParameter("algoLatency", xdata::UnsignedInteger(0x0));
  registerParameter("internalLatency", xdata::UnsignedInteger(0x0));
  registerParameter("rxExtraFrames", xdata::UnsignedInteger(0x0));
  registerParameter("txExtraFrames", xdata::UnsignedInteger(0x0));
}



// --------------------------------------------------------
template<class Selector>
core::Command::State EasyLatencyCommand<Selector>::code(const ::swatch::core::XParameterSet& params)
{
  // Get the corresponding buffer kind
  const ::mp7::RxTxSelector bKind = BufferTraits<Selector>::kRxTxSelector;

  uint bankId = params.get<xdata::UnsignedInteger>("bankId").value_;
  uint masterLatency = params.get<xdata::UnsignedInteger>("masterLatency").value_;
  uint algoLatency = params.get<xdata::UnsignedInteger>("algoLatency").value_;
  uint internalLatency = params.get<xdata::UnsignedInteger>("internalLatency").value_;
  uint rxExtraFrames = params.get<xdata::UnsignedInteger>("rxExtraFrames").value_;
  uint txExtraFrames = params.get<xdata::UnsignedInteger>("txExtraFrames").value_;

  setProgress(0.0, "Configuring " + boost::lexical_cast<std::string>(bKind) + " buffers in latency mode");

  uint32_t depth = computeLatency(masterLatency, algoLatency, internalLatency, rxExtraFrames, txExtraFrames);
  
  ::mp7::ChannelManager cm = mBufferSelector.manager(params);
  ::mp7::LatencyPathConfigurator pc = ::mp7::LatencyPathConfigurator(bankId, depth);

  cm.configureBuffers(bKind, pc);

  setStatusMsg(boost::lexical_cast<std::string>(bKind) + " buffers configured in latency mode: depth = " + boost::lexical_cast<std::string>(depth));

  return State::kDone;
}


template<>
uint32_t
EasyLatencyCommand<RxBufferSelector>::computeLatency(uint32_t aMaster, uint32_t aAlgo, uint32_t aInternal, uint32_t aRxExtraFrames, uint32_t aTxExtraFrames)
{
  //  return aMaster + aInternal + aRxExtraFrames;
  uint32_t lLatency = aMaster + aInternal + aRxExtraFrames;
  LOG4CPLUS_INFO(getActionable<MP7AbstractProcessor>().getLogger(), "Compute Rx Latency:" 
      << " M(" << aMaster 
      << ")+I(" << aInternal
      << ")+xRx(" << aRxExtraFrames
      << ") => L = " << lLatency);

  return lLatency;
}


template<>
uint32_t
EasyLatencyCommand<TxBufferSelector>::computeLatency(uint32_t aMaster, uint32_t aAlgo, uint32_t aInternal, uint32_t aRxExtraFrames, uint32_t aTxExtraFrames)
{
  uint32_t lLatency = aMaster + aInternal - aAlgo + aTxExtraFrames;

    LOG4CPLUS_INFO(getActionable<MP7AbstractProcessor>().getLogger(), "Compute Tx Latency:" 
      << " M(" << aMaster 
      << ")+I(" << aInternal
      << ")-A(" << aAlgo
      << ")+xTx(" << aRxExtraFrames
      << ") => L = " << lLatency);

  return lLatency;
}

template class EasyLatencyCommand<RxBufferSelector>;
template class EasyLatencyCommand<TxBufferSelector>;


} //end ns mp7
} //end ns swatch
