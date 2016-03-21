
#include "swatch/mp7/FormatterCommands.hpp"


#include <iostream>
#include <sstream>
#include <fstream>

#include "swatch/mp7/MP7AbstractProcessor.hpp"

// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/processor/PortCollection.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/core/toolbox/IntListParser.hpp"

// XDAQ Headers
#include "xdata/Boolean.h"
#include "xdata/Integer.h"
#include "xdata/UnsignedInteger.h"
#include "xdata/String.h"

// uHAL Headers
#include "uhal/HwInterface.hpp"

// C++ Headers
#include <iomanip>


// MP7 Core Headers
#include "mp7/MP7Controller.hpp"
#include "mp7/PathConfigurator.hpp"
#include "mp7/BoardData.hpp"
#include "mp7/exception.hpp"
#include "mp7/FormatterNode.hpp"
#include "mp7/DatapathNode.hpp"
#include "swatch/mp7/Utilities.hpp"


namespace swatch {
namespace mp7 {

HeaderFormatterCommand::HeaderFormatterCommand(const std::string& aId, swatch::core::ActionableObject& aActionable):
  swatch::core::Command(aId, aActionable, xdata::String()), 
  mFmtSelector(*this, 
    (boost::bind(&channel::Descriptor::getFormatterKind, _1) == ::mp7::kTDRFormatter) ||
    (boost::bind(&channel::Descriptor::getFormatterKind, _1) == ::mp7::kDemuxFormatter)
    ) {
  
  // add default parameters
  mFmtSelector.addCommandParameters();
  
  registerParameter("strip", xdata::Boolean(true));
  registerParameter("insert", xdata::Boolean(true));
}

//---
core::Command::State HeaderFormatterCommand::code(const swatch::core::XParameterSet& params)
{

  bool strip  = (bool)params.get<xdata::Boolean>("strip");
  bool insert = (bool)params.get<xdata::Boolean>("insert");

  ::mp7::ChannelManager cm = mFmtSelector.manager(params);

  setProgress(0.0, "Configuring header formatting...");
  
  // 
  cm.configureHdrFormatters(::mp7::kTDRFormatter, strip,insert);
  cm.configureHdrFormatters(::mp7::kDemuxFormatter, strip,insert);

  setStatusMsg("Configure TDR Formatting complete");

  return State::kDone;
}  




DatavalidFormatterCommand::DatavalidFormatterCommand(const std::string& aId, swatch::core::ActionableObject& aActionable):
  swatch::core::Command(aId, aActionable, xdata::String()),
  mFmtSelector(*this, boost::bind(&channel::Descriptor::getFormatterKind, _1) == ::mp7::kDemuxFormatter) {

    // add default parameters
  mFmtSelector.addCommandParameters();
  
  registerParameter("startBx", xdata::UnsignedInteger(0x0));
  registerParameter("startCycle", xdata::UnsignedInteger(0x0));
  registerParameter("stopBx", xdata::UnsignedInteger(0x0));
  registerParameter("stopCycle", xdata::UnsignedInteger(0x0));
}


swatch::core::Command::State DatavalidFormatterCommand::code(const swatch::core::XParameterSet& params)
{

  const xdata::UnsignedInteger& startBx    =  params.get<xdata::UnsignedInteger>("startBx");
  const xdata::UnsignedInteger& startCycle =  params.get<xdata::UnsignedInteger>("startCycle");
  const xdata::UnsignedInteger& stopBx     =  params.get<xdata::UnsignedInteger>("stopBx");
  const xdata::UnsignedInteger& stopCycle  =  params.get<xdata::UnsignedInteger>("stopCycle");


  ::mp7::MP7Controller& driver = mFmtSelector.getDriver();
  ::mp7::ChannelManager cm =  mFmtSelector.manager(params);
  ::mp7::orbit::Metric m = driver.getMetric();

  // Check dv begin orbitpoint
  if (!orbit::isValid(startBx, startCycle, m)) {
    std::ostringstream msg;
    msg << "Invalid start orbit point (" << startBx<< ", " << startCycle << ")";

    setStatusMsg(msg.str());
    return State::kError;
  }
  
  // Check dv end orbitpoint
  if (!orbit::isValid(stopBx, stopCycle, m)) {
    std::ostringstream msg;
    msg << "Invalid stop orbit point (" << stopBx << ", " << stopCycle << ")";

    setStatusMsg(msg.str());
    return State::kError;
  }
  

  ::mp7::orbit::Point first = ::mp7::orbit::Point((uint32_t)startBx,(uint32_t)startCycle);
  ::mp7::orbit::Point last = ::mp7::orbit::Point((uint32_t)stopBx,(uint32_t)stopCycle);

  // Fixme: this is always true
  if(not (
      startBx.isNaN() and 
      stopBx.isNaN() and
      startCycle.isNaN() and
      stopCycle.isNaN() 
      ) 
    ){

    setProgress(0.4, "Configuring data valid override...");
    cm.configureDVFormatters(first, last);
  } else {
    setProgress(0.4, "Disabling data valid override...");
    cm.disableDVFormatters();
  }
  setStatusMsg("Configure data valid complete");
  return State::kDone;
}



//
//
// Old code
//
//
//
//TDRFormatterCommand::TDRFormatterCommand(const std::string& aId, swatch::core::ActionableObject& aActionable):
//  swatch::core::Command(aId, aActionable, xdata::Integer()), 
//  mFmtSelector(*this, boost::bind(&channel::Descriptor::getFormatterKind, _1) == ::mp7::kTDRFormatter) {
//  
//  // add default parameters
//  mFmtSelector.addCommandParameters();
//  
//  registerParameter("strip", xdata::Boolean(true));
//  registerParameter("insert", xdata::Boolean(true));
//}
//
////---
//core::Command::State TDRFormatterCommand::code(const swatch::core::XParameterSet& params)
//{
//
//  bool strip  = params.get<xdata::Boolean>("strip").value_;
//  bool insert = params.get<xdata::Boolean>("insert").value_;
//
////  ::mp7::MP7Controller& driver = mFmtSelector.getDriver();
//  ::mp7::ChannelManager cm = mFmtSelector.manager(params);
//
//  setProgress(0.0, "Configuring TDR header formatting...");
//  
//  // Bugged in mp7sw 1.8.4 and 1.8.4
//  // Will be fixed in 1.9.X
//  cm.configureHdrFormatters(::mp7::kTDRFormatter, strip,insert);
//
//
////  //FIXME: Patch starts
//// ::mp7::ChannelGroup tdrChans = cm.getDescriptor().pickFmtIDs(::mp7::kTDRFormatter);
//// 
//// ::mp7::FormatterNode fmt = driver.getFormatter();
//// ::mp7::DatapathNode datapath = driver.getDatapath();
////
//// setProgress(0.2, "Configuring TDR header formatting...");
////
//// BOOST_FOREACH( uint32_t ch, tdrChans.channels() ) { 
//// // for(uint i = 0; i < tdrChans.channels().size(); i++){
////   // datapath.selectLink(tdrChans.channels()[i]);
////   datapath.selectLink(ch);
////   fmt.stripInsert(strip, insert);
//// }
//
//  //FIXME: Patch ends
//
//  setStatusMsg("Configure TDR Formatting complete");
//
//  return State::kDone;
//}
//
////
////
////
////
//
//
//DemuxFormatterCommand::DemuxFormatterCommand(const std::string& aId, swatch::core::ActionableObject& aActionable):
//  swatch::core::Command(aId, aActionable, xdata::Integer()),
//  mFmtSelector(*this, boost::bind(&channel::Descriptor::getFormatterKind, _1) == ::mp7::kDemuxFormatter) {
//
//    // add default parameters
//  mFmtSelector.addCommandParameters();
//  
//  registerParameter("strip", xdata::Boolean(true));
//  registerParameter("insert", xdata::Boolean(true));
//  registerParameter("startBx", xdata::UnsignedInteger(0x0));
//  registerParameter("startCycle", xdata::UnsignedInteger(0x0));
//  registerParameter("stopBx", xdata::UnsignedInteger(0x0));
//  registerParameter("stopCycle", xdata::UnsignedInteger(0x0));
//}
//
//
//swatch::core::Command::State DemuxFormatterCommand::code(const swatch::core::XParameterSet& params)
//{
//
//  bool strip  = params.get<xdata::Boolean>("strip").value_;
//  bool insert = params.get<xdata::Boolean>("insert").value_;
//  const xdata::UnsignedInteger& startBx    =  params.get<xdata::UnsignedInteger>("startBx");
//  const xdata::UnsignedInteger& startCycle =  params.get<xdata::UnsignedInteger>("startCycle");
//  const xdata::UnsignedInteger& stopBx     =  params.get<xdata::UnsignedInteger>("stopBx");
//  const xdata::UnsignedInteger& stopCycle  =  params.get<xdata::UnsignedInteger>("stopCycle");
//
//
//  ::mp7::MP7Controller& driver = mFmtSelector.getDriver();
//  ::mp7::ChannelManager cm =  mFmtSelector.manager(params);
//  ::mp7::orbit::Metric m = driver.getMetric();
//
//  // Check dv begin orbitpoint
//  if (!orbit::isValid(startBx, startCycle, m)) {
//    std::ostringstream msg;
//    msg << "Invalid start orbit point (" << startBx.value_ << ", " << startCycle.value_ << ")";
//
//    setStatusMsg(msg.str());
//    return State::kError;
//  }
//  
//  // Check dv end orbitpoint
//  if (!orbit::isValid(stopBx, stopCycle, m)) {
//    std::ostringstream msg;
//    msg << "Invalid stop orbit point (" << stopBx.value_ << ", " << stopCycle.value_ << ")";
//
//    setStatusMsg(msg.str());
//    return State::kError;
//  }
//  
//  setProgress(0.1, "Configuring formatters");
//
//  // If the header is re-inserted, the data is delayed by 1 clock cycle
//  // Do I need to correct the outgoing range?
//  // Offset the formatter by 1 cycle
//  ::mp7::orbit::Point first = m.subCycles(::mp7::orbit::Point(startBx.value_,startCycle.value_),1);
//  ::mp7::orbit::Point last = m.subCycles(::mp7::orbit::Point(stopBx.value_,stopCycle.value_),1);
//
//  // Bugged in mp7sw 1.8.4 and 1.8.4
//  // Will be fixed in 1.9.X
//  // cm.configureDemuxFormatters(strip, insert, first, last);
//
//  //FIXME: Patch starts
//  ::mp7::ChannelGroup dmxIds = cm.getDescriptor().pickFmtIDs(::mp7::kDemuxFormatter);
//
//  ::mp7::FormatterNode fmt = driver.getFormatter();
//  ::mp7::DatapathNode datapath = driver.getDatapath();
//
//  setProgress(0.2, "Configuring Demux header formatting...");
//
////  BOOST_FOREACH( uint32_t ch, dmxIds.channels() ) { 
////   datapath.selectLink(ch);
////   fmt.stripInsert(strip, insert);
////  }
//  cm.configureHdrFormatters(::mp7::kDemuxFormatter, strip, insert);
//  
//  // Fixme: this is always true
//  if(not (
//      startBx.isNaN() and 
//      stopBx.isNaN() and
//      startCycle.isNaN() and
//      stopCycle.isNaN() 
//      ) 
//    ){
//
//    setProgress(0.4, "Configuring Demux data valid override...");
//    cm.configureDVFormatters(first, last);
////    ::mp7::orbit::Metric m = driver.getMetric();
////    BOOST_FOREACH( uint32_t ch, dmxIds.channels() ) { 
////      datapath.selectLink(ch);
////      fmt.overrideValid(first, last);
////    }
//  } else {
//    setProgress(0.4, "Disabling Demux data valid override...");
//    cm.disableDVFormatters();
////    BOOST_FOREACH( uint32_t ch, dmxIds.channels() ) { 
////      datapath.selectLink(ch);
////      fmt.enableValidOverride(false);
////   }
//
//   setStatusMsg("Configure Demux Formatting complete");
//  }
// //FIXME: Patch ends
//  return State::kDone;
//}
//
////
//// S1 Formatter
////
//S1Formatter::S1Formatter(const std::string& aId, swatch::core::ActionableObject& aActionable):
//  swatch::core::Command(aId, aActionable, xdata::Integer()),
//  mFmtSelector(*this, boost::bind(&channel::Descriptor::getFormatterKind, _1) == ::mp7::kStage1Formatter)
//{
//    // add default parameters
//  mFmtSelector.addCommandParameters();
//  
//  registerParameter("s1BC0Bx", xdata::UnsignedInteger(0x0));
//  registerParameter("s1BC0Cycle", xdata::UnsignedInteger(0x0));
//}
//
//
//
//swatch::core::Command::State S1Formatter::code(const swatch::core::XParameterSet& params)
//{
//
//  MP7AbstractProcessor& mp7 = getActionable< MP7AbstractProcessor>();
//  
//  uint s1BC0Bx =  params.get<xdata::UnsignedInteger>("s1BC0Bx").value_;
//  uint s1BC0Cycle = params.get<xdata::UnsignedInteger>("s1BC0Cycle").value_; 
//
//
//  ::mp7::ChannelManager cm =  mFmtSelector.manager(params);
//
//  setProgress(0.0, "Configuring formatters");
//  
//  
//  ::mp7::FormatterNode fmt = mp7.driver().getFormatter();
//  ::mp7::DatapathNode datapath = mp7.driver().getDatapath();
//
//  setProgress(0.2, "Configuring Stage-1 header formatting...");
//  
//  if(s1BC0Bx || s1BC0Cycle){
//    ::mp7::orbit::Metric m = mp7.driver().getMetric();
//    std::ostringstream bc0;
//    bc0 << "Tagging BC0 at " << s1BC0Bx << ", " << s1BC0Cycle << "...";
//    setProgress(0.4, bc0.str());
//    ::mp7::orbit::Point bx = m.subCycles(::mp7::orbit::Point(s1BC0Bx,s1BC0Cycle),1);
//    BOOST_FOREACH(uint32_t r, cm.getDescriptor().pickFmtIDs(::mp7::kStage1Formatter).regions()) {
//      datapath.selectRegion(r);
//      fmt.tagBC0(bx);
//    }
//
//    setStatusMsg("Configure Stage-1 Formatting complete");
//						 
//  }
//  return State::kDone;
//
//}

} // namespace mp7
} // namespace swatch
