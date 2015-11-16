
#include "swatch/mp7/BufferCommands.hpp"


// STL headers
#include <sstream>
#include <iomanip>

// boost headers
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

// XDAQ headers
#include "xdata/String.h"
#include "xdata/UnsignedInteger.h"

// MP7 Core Headers
#include "mp7/BoardData.hpp"
#include "mp7/MP7Controller.hpp"
#include "mp7/PathConfigurator.hpp"
#include "mp7/TTCNode.hpp"
#include "mp7/operators.hpp"

// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/mp7/MP7Processor.hpp"


namespace swatch {
namespace mp7 {


// Static initialization
template<AbstractChannelsCommand::ChannelGroup tGroup>
const std::map< std::string, ::mp7::TestPathConfigurator::Mode > ConfigureBuffersCommand<tGroup>::mBufferModeMap = initBufferModeMap();

// Template specialisation
template<>
const ::mp7::BufferKind BufferTraits<AbstractChannelsCommand::kRx>::bufferKind = ::mp7::kRxBuffer;

template<>
const ::mp7::BufferKind BufferTraits<AbstractChannelsCommand::kTx>::bufferKind = ::mp7::kTxBuffer;

// --------------------------------------------------------
template<AbstractChannelsCommand::ChannelGroup tGroup>
std::map< std::string, ::mp7::TestPathConfigurator::Mode >
ConfigureBuffersCommand<tGroup>::initBufferModeMap() {

  std::map< std::string, ::mp7::TestPathConfigurator::Mode > lModes;

  lModes["Latency"] = ::mp7::TestPathConfigurator::kLatency;
  lModes["Capture"] = ::mp7::TestPathConfigurator::kCapture;
  lModes["PlayOnce"] = ::mp7::TestPathConfigurator::kPlayOnce;
  lModes["PlayLoop"] = ::mp7::TestPathConfigurator::kPlayLoop;
  lModes["Pattern"] = ::mp7::TestPathConfigurator::kPattern;
  lModes["Zeroes"] = ::mp7::TestPathConfigurator::kZeroes;
  lModes["CaptureStrobe"] = ::mp7::TestPathConfigurator::kCaptureStrobe;
  lModes["Pattern3G"] = ::mp7::TestPathConfigurator::kPattern3G;
  lModes["PlayOnceStrobe"] = ::mp7::TestPathConfigurator::kPlayOnceStrobe;
  lModes["PlayOnce3G"] = ::mp7::TestPathConfigurator::kPlayOnce3G;

  return lModes;
}


// --------------------------------------------------------
template<AbstractChannelsCommand::ChannelGroup tGroup>
ConfigureBuffersCommand<tGroup>::ConfigureBuffersCommand(const std::string& aId) :
AbstractChannelsCommand(aId, tGroup, xdata::String() ) {

  // Parameter registration  
  registerParameter("startBx", xdata::UnsignedInteger(0x0));
  registerParameter("startCycle", xdata::UnsignedInteger(0x0));
  registerParameter("stopBx", xdata::UnsignedInteger());
  registerParameter("stopCycle", xdata::UnsignedInteger());
  registerParameter("payload", xdata::String("file:///path/to/file"));
  registerParameter("mode", xdata::String(""));
}


// --------------------------------------------------------
template<AbstractChannelsCommand::ChannelGroup tGroup>
ConfigureBuffersCommand<tGroup>::~ConfigureBuffersCommand() {
}


// --------------------------------------------------------
template<AbstractChannelsCommand::ChannelGroup tGroup>
core::Command::State
ConfigureBuffersCommand<tGroup>::code(const ::swatch::core::XParameterSet& params) {

  const ::mp7::BufferKind bKind = BufferTraits<tGroup>::bufferKind;

  // Extract parameter values
  // LOG(swatch::logger::kWarning) << params;

  std::string modeValue = params.get<xdata::String>("mode").value_;

  uint startBxVal = params.get<xdata::UnsignedInteger>("startBx").value_;
  uint startCycleVal = params.get<xdata::UnsignedInteger>("startCycle").value_;

  uint stopBxVal = params.get<xdata::UnsignedInteger>("stopBx").value_;
  uint stopCycleVal  = params.get<xdata::UnsignedInteger>("stopCycle").value_;

  std::string payload = params.get<xdata::String>("payload").value_;

  // TODO: make a generic bx/cycle checker

  const xdata::UnsignedInteger& startBx = params.get<xdata::UnsignedInteger>("startBx");
  const xdata::UnsignedInteger& startCycle = params.get<xdata::UnsignedInteger>("startCycle");
  const xdata::UnsignedInteger& stopBx = params.get<xdata::UnsignedInteger>("stopBx");
  const xdata::UnsignedInteger& stopCycle = params.get<xdata::UnsignedInteger>("stopCycle");

  // Instantiate the message streamer once
  std::ostringstream msg;
  // Extract the MP7 driver
  ::mp7::MP7Controller& driver = getActionable< ::swatch::mp7::MP7Processor>()->driver();
  ::mp7::orbit::Metric metric = driver.getMetric();

  // TOFIX: make check standard
  if ( !orbit::isValid( startBx, startCycle, metric ) ) {
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

  if ( !payload.empty() )
    msg << " reading from: " << payload;

  setStatusMsg(msg.str());

  ::mp7::ChannelsManager cm = getChannelsMgr(params);

  setProgress(0.2, "Generating BoardData object...");

  // Reset msg
  msg.str("");
  // Populate message
  msg << "Setting range : start point " << startBx.value_ << ", " << startCycle.value_;
  setProgress(0.4, msg.str());

  ::mp7::TestPathConfigurator::Mode lBufMode;
  try {
    lBufMode = mBufferModeMap.at(modeValue);
  } catch ( std::out_of_range& e ) {
    std::ostringstream msg;
    msg << "Unknown buffer mode: '" << modeValue << "'";
    setStatusMsg(msg.str());
    return State::kError;
  }

  ::mp7::orbit::Point startPoint = ::mp7::orbit::Point(startBxVal, startCycleVal);

  // Create a base configurator
  ::mp7::TestPathConfigurator pc = ::mp7::TestPathConfigurator(lBufMode, startPoint, metric);

  // If stop is defined, update it
  if ( !orbit::isNull(stopBx, stopCycle) )  {
    
    // Ensure the end orbit point is good
    if ( !orbit::isValid( stopBx, stopCycle, metric ) ) {
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

  if ( !payload.empty() ) {
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


template class ConfigureBuffersCommand<AbstractChannelsCommand::kRx>;
template class ConfigureBuffersCommand<AbstractChannelsCommand::kTx>;



// --------------------------------------------------------
CaptureBuffersCommand::CaptureBuffersCommand(const std::string& aId) :
  swatch::core::Command(aId, xdata::String() ) {
}


// --------------------------------------------------------
CaptureBuffersCommand::~CaptureBuffersCommand() {
}


// --------------------------------------------------------
core::Command::State
CaptureBuffersCommand::code(const ::swatch::core::XParameterSet& params) {

  ::mp7::MP7Controller& driver = getActionable< ::swatch::mp7::MP7Processor>()->driver();

  ::mp7::TTCNode ttc = driver.getTTC();
  ::mp7::ChannelsManager cm = driver.channelMgr();


  setProgress(0.5, "Clearing capture buffers ...");

  cm.clearBuffers(::mp7::kRxBuffer, ::mp7::ChanBufferNode::kCapture);
  cm.clearBuffers(::mp7::kTxBuffer, ::mp7::ChanBufferNode::kCapture);

  setProgress(0.5, "Capturing data stream ...");

  ttc.forceBTest();
  cm.waitCaptureDone();

  setStatusMsg("Capture completed");
  return State::kDone;
}



// --------------------------------------------------------
template<AbstractChannelsCommand::ChannelGroup tGroup>
SaveBuffersToFileCommand<tGroup>::SaveBuffersToFileCommand(const std::string& aId) :
  AbstractChannelsCommand(aId, tGroup, xdata::String() ) {
  registerParameter("filename", xdata::String(""));
}


// --------------------------------------------------------
template<AbstractChannelsCommand::ChannelGroup tGroup>
SaveBuffersToFileCommand<tGroup>::~SaveBuffersToFileCommand() {
}


// --------------------------------------------------------
template<AbstractChannelsCommand::ChannelGroup tGroup>
core::Command::State SaveBuffersToFileCommand<tGroup>::code(const ::swatch::core::XParameterSet& params) {

  // Get the corresponding buffer kind
  const ::mp7::BufferKind bKind = BufferTraits<tGroup>::bufferKind;

  std::string filename = params.get<xdata::String>("filename").value_;

//  swatch::mp7::MP7Processor* proc = getParent< ::swatch::mp7::MP7Processor>();
  ::mp7::MP7Controller& driver = getActionable< ::swatch::mp7::MP7Processor>()->driver();

  ::mp7::CtrlNode ctrl = driver.getCtrl();
  ::mp7::ChannelsManager cm = getChannelsMgr(params);

  
  // TOFIX: Output file should be compulsory
  if (filename == "") {
    std::ostringstream fn;
    fn << bKind << "_summary.txt";
    filename = fn.str();
  } else {
    // Expand variables contained in the filename
    filename = swatch::core::shellExpandPath( filename );
    
    boost::filesystem::path p(filename);
    if (!boost::filesystem::exists(p.parent_path())) {
      try {
        boost::filesystem::create_directories(p.parent_path());
      } catch (std::exception& e)  {
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
template class SaveBuffersToFileCommand<AbstractChannelsCommand::kRx>;
template class SaveBuffersToFileCommand<AbstractChannelsCommand::kTx>;



// --------------------------------------------------------
template<AbstractChannelsCommand::ChannelGroup tGroup>
LatencyBuffersCommand<tGroup>::LatencyBuffersCommand(const std::string& aId) :
  AbstractChannelsCommand(aId, tGroup, xdata::String() ) {
  registerParameter("bankId", xdata::UnsignedInteger(0x0));
  registerParameter("depth", xdata::UnsignedInteger(0x0));
}


// --------------------------------------------------------
template<AbstractChannelsCommand::ChannelGroup tGroup>
LatencyBuffersCommand<tGroup>::~LatencyBuffersCommand() {
}


// --------------------------------------------------------
template<AbstractChannelsCommand::ChannelGroup tGroup>
core::Command::State LatencyBuffersCommand<tGroup>::code(const ::swatch::core::XParameterSet& params) {
  // Get the corresponding buffer kind
  const ::mp7::BufferKind bKind = BufferTraits<tGroup>::bufferKind;


  uint bankId = params.get<xdata::UnsignedInteger>("bankId").value_;
  uint depth = params.get<xdata::UnsignedInteger>("depth").value_;

  setProgress(0.0, "Configuring buffers in latency mode");

  ::mp7::ChannelsManager cm = getChannelsMgr(params);
  ::mp7::LatencyPathConfigurator pc = ::mp7::LatencyPathConfigurator(bankId, depth);

  cm.configureBuffers(bKind, pc);

  setStatusMsg("Finished configuring buffers in latency mode");

  return State::kDone;
}

template class LatencyBuffersCommand<AbstractChannelsCommand::kRx>;
template class LatencyBuffersCommand<AbstractChannelsCommand::kTx>;


// --------------------------------------------------------
template<AbstractChannelsCommand::ChannelGroup tGroup>
EasyLatencyCommand<tGroup>::EasyLatencyCommand(const std::string& aId) :
  AbstractChannelsCommand(aId, tGroup, xdata::String() ) {
  registerParameter("bankId", xdata::UnsignedInteger(0x0));
  registerParameter("masterLatency", xdata::UnsignedInteger(0x0));
  registerParameter("algoLatency", xdata::UnsignedInteger(0x0));
  registerParameter("internalLatency", xdata::UnsignedInteger(0x0));
}


// --------------------------------------------------------
template<AbstractChannelsCommand::ChannelGroup tGroup>
EasyLatencyCommand<tGroup>::~EasyLatencyCommand() {
}


// --------------------------------------------------------
template<AbstractChannelsCommand::ChannelGroup tGroup>
core::Command::State EasyLatencyCommand<tGroup>::code(const ::swatch::core::XParameterSet& params) {
  // Get the corresponding buffer kind
  const ::mp7::BufferKind bKind = BufferTraits<tGroup>::bufferKind;

  uint bankId = params.get<xdata::UnsignedInteger>("bankId").value_;
  uint masterLatency = params.get<xdata::UnsignedInteger>("masterLatency").value_;
  uint algoLatency = params.get<xdata::UnsignedInteger>("algoLatency").value_;
  uint internalLatency = params.get<xdata::UnsignedInteger>("internalLatency").value_;

  setProgress(0.0, "Configuring "+boost::lexical_cast<std::string>(bKind)+" buffers in latency mode");

  uint32_t depth = computeLatency(masterLatency, algoLatency, internalLatency);

  ::mp7::ChannelsManager cm = getChannelsMgr(params);
  ::mp7::LatencyPathConfigurator pc = ::mp7::LatencyPathConfigurator(bankId, depth);

  cm.configureBuffers(bKind, pc);

  setStatusMsg(boost::lexical_cast<std::string>(bKind)+" buffers configured in latency mode: depth = "+boost::lexical_cast<std::string>(depth));

  return State::kDone;
}

template<>
uint32_t 
EasyLatencyCommand<AbstractChannelsCommand::kRx>::computeLatency(uint32_t aMaster, uint32_t aAlgo, uint32_t aInternal) {
  return aMaster+aInternal;
}


template<>
uint32_t 
EasyLatencyCommand<AbstractChannelsCommand::kTx>::computeLatency(uint32_t aMaster, uint32_t aAlgo, uint32_t aInternal) {
  return aMaster+aInternal-aAlgo;
}

template class EasyLatencyCommand<AbstractChannelsCommand::kRx>;
template class EasyLatencyCommand<AbstractChannelsCommand::kTx>;


} //end ns mp7
} //end ns swatch
