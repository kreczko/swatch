
#include "swatch/mp7/FormatterCommands.hpp"


#include <iostream>
#include <sstream>
#include <fstream>

#include "swatch/mp7/MP7Processor.hpp"

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

// MP7 Headers
#include "mp7/MP7Controller.hpp"

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


namespace swatch {
namespace mp7 {

  
AbstractFormatterCommand::~AbstractFormatterCommand()
{
}


::mp7::ChannelsManager AbstractFormatterCommand::getChannelsMgr(const swatch::core::XParameterSet& aParams)
{
  // Parse parameter into channel ID mask
  std::string channelMask = aParams.get<xdata::String>(kPortSelection).value_;
  std::vector<uint32_t> mask;
  if (channelMask.empty())
  {
    mask.reserve(72);
    for(size_t i=0; i<72; i++)
        mask.push_back(i);
  }
  else
    mask = swatch::core::toolbox::UIntListParser::parse(channelMask);

  // Grab list of ports registered in the processor
  swatch::mp7::MP7Processor* p = getParent<swatch::mp7::MP7Processor>();
 
 // const std::vector<swatch::processor::ProcessorPortStub>& portStubs = (mKind == mp7::kRxBuffer) ? p->getStub().rxPorts : p->getStub().txPorts;
  // std::vector<uint32_t> stubIds;
  // for(std::vector<swatch::processor::ProcessorPortStub>::const_iterator lIt=portStubs.begin(); lIt!=portStubs.end(); lIt++)
  //   stubIds.push_back( lIt->number );

  // // Obtain intersection of the two lists
  // std::sort(mask.begin(), mask.end());
  // std::sort(stubIds.begin(), stubIds.end());
  // std::vector<uint32_t> intersection;
  // std::set_intersection(mask.begin(), mask.end(), stubIds.begin(), stubIds.end(), std::back_inserter(intersection));

  //return p->driver().channelMgr(intersection);

  return p->driver().channelMgr(mask);

}

const std::string AbstractFormatterCommand::kPortSelection = "portSelection";  





TDRFormatterCommand::TDRFormatterCommand(const std::string& aId):
  AbstractFormatterCommand(aId, xdata::Integer()){
  registerParameter("strip", xdata::Boolean(true));
  registerParameter("insert", xdata::Boolean(true));
}

TDRFormatterCommand::~TDRFormatterCommand(){
}

core::Command::State TDRFormatterCommand::code(const ::swatch::core::XParameterSet& params)
{

  ::swatch::mp7::MP7Processor* mp7 = getParent< ::swatch::mp7::MP7Processor>();

  bool strip  = params.get<xdata::Boolean>("strip").value_;
  bool insert = params.get<xdata::Boolean>("insert").value_;


  // Need to parse enabled channels

  ::mp7::ChannelsManager cm = getChannelsMgr(params);

  setProgress(0.0, "Configuring formatters");
  
  ::mp7::ChannelIDSet tdrIds = cm.ids(::mp7::kTDRFmtIDs);
  
  ::mp7::FormatterNode fmt = mp7->driver().getFormatter();
  ::mp7::CtrlNode ctrl = mp7->driver().getCtrl();
  ::mp7::DatapathNode datapath = mp7->driver().getDatapath();

  setProgress(0.2, "Configuring TDR header formatting...");
  
  for(uint i = 0; i < tdrIds.channels().size(); i++){
    datapath.selectLink(tdrIds.channels()[i]);
    fmt.stripInsert(strip, insert);
  }

  setStatusMsg("Configure TDR Formatting complete");

  return State::kDone;
}


} //end ns mp7
} //end ns swatch
