
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

  
TDRFormatterCommand::TDRFormatterCommand(const std::string& aId, swatch::core::ActionableObject& aActionable):
  ChannelCommandBase(aId, aActionable, xdata::Integer()), 
  mCore(*this, boost::bind(&ChannelDescriptor::getFormatterKind, _1) == ::mp7::kTDRFormatter) {
  
  // add default parameters
  mCore.addParameters();
  
  registerParameter("strip", xdata::Boolean(true));
  registerParameter("insert", xdata::Boolean(true));
}

//---
core::Command::State TDRFormatterCommand::code(const ::swatch::core::XParameterSet& params)
{

  bool strip  = params.get<xdata::Boolean>("strip").value_;
  bool insert = params.get<xdata::Boolean>("insert").value_;


  // Need to parse enabled channels

  ::mp7::ChannelsManager cm = mCore.getManager(params);

  setProgress(0.0, "Configuring TDR header formatting...");
  
  cm.configureTDRFormatters(strip,insert);

  setStatusMsg("Configure TDR Formatting complete");

  return State::kDone;
}


} //end ns mp7
} //end ns swatch
