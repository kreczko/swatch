
#include "swatch/mp7/MGTCommands.hpp"


#include <string>
#include <iostream>

#include "xdata/String.h"
#include "xdata/Integer.h"
#include "xdata/UnsignedInteger.h"
#include "xdata/Boolean.h"

#include "mp7/MP7Controller.hpp"
#include "mp7/TTCNode.hpp"
#include "mp7/PathConfigurator.hpp"
#include "mp7/Orbit.hpp"

#include "swatch/mp7/MP7Processor.hpp"
#include "swatch/core/toolbox/IntListParser.hpp"


namespace swatch {
namespace mp7 {


// --------------------------------------------------------
ConfigureRxMGTsCommand::ConfigureRxMGTsCommand(const std::string& aId) :
AbstractChannelsCommand(aId, AbstractChannelsCommand::kRx, xdata::String()) {
  registerParameter("orbitTag", xdata::Boolean(true));
  registerParameter("polarity", xdata::Boolean(false));

}


// --------------------------------------------------------
ConfigureRxMGTsCommand::~ConfigureRxMGTsCommand() {
}


// --------------------------------------------------------
swatch::core::Command::State
ConfigureRxMGTsCommand::code(const swatch::core::XParameterSet& params) {
  bool orbitTag = params.get<xdata::Boolean>("orbitTag").value_;
  bool polarity = params.get<xdata::Boolean>("polarity").value_;

  ::mp7::ChannelsManager cm = getChannelsMgr(params);

  setProgress(0.0, "Configuring Rx MGTs...");

  try {
    cm.configureRxMGTs(orbitTag, polarity);
  }  catch (std::exception& e) {
    std::ostringstream err;
    err << "Exception caught while trying to configure Rx MGTs: " << e.what();
    setStatusMsg(err.str());
    return kError;
  }

  setStatusMsg("Configure Rx MGTs complete");
  return kDone;
}

// --------------------------------------------------------
ConfigureTxMGTsCommand::ConfigureTxMGTsCommand(const std::string& aId) :
AbstractChannelsCommand(aId, AbstractChannelsCommand::kTx, xdata::String()) {
  registerParameter("orbitTag", xdata::Boolean(true));
  registerParameter("loopback", xdata::Boolean(false));
  registerParameter("polarity", xdata::Boolean(false));

}

// --------------------------------------------------------
ConfigureTxMGTsCommand::~ConfigureTxMGTsCommand() {
}

// --------------------------------------------------------
swatch::core::Command::State
ConfigureTxMGTsCommand::code(const swatch::core::XParameterSet& params) {
 bool orbitTag = params.get<xdata::Boolean>("orbitTag").value_;
  bool loopback = params.get<xdata::Boolean>("loopback").value_;
  bool polarity = params.get<xdata::Boolean>("polarity").value_;

  ::mp7::ChannelsManager cm = getChannelsMgr(params);

  setProgress(0.0, "Configuring Tx MGTs");

    if (loopback) {
      setProgress(0.1, "Configuring links in loopback mode...");
      if (orbitTag)
        cm.setupTx2RxOrbitPattern();
      else
        cm.setupTx2RxPattern();
    }

    setProgress(0.3, "Configuring links...");

  try {
    cm.configureTxMGTs(loopback, polarity);
  }  catch (std::exception& e) {
    std::ostringstream err;
    err << "Exception caught while trying to configure Tx MGTs: " << e.what();
    setStatusMsg(err.str());
    return kError;
  }

  setStatusMsg("Configure Tx MGTs completed");
  return kDone;
}


// --------------------------------------------------------
AlignRxsToCommand::AlignRxsToCommand(const std::string& aId) :
AbstractChannelsCommand(aId, AbstractChannelsCommand::kRx, xdata::String())
{
  registerParameter("bx", xdata::UnsignedInteger(0x0));
  registerParameter("cycle", xdata::UnsignedInteger(0x0));
}


// --------------------------------------------------------
AlignRxsToCommand::~AlignRxsToCommand() {
}


// --------------------------------------------------------
core::Command::State
AlignRxsToCommand::code(const swatch::core::XParameterSet& params) {

  xdata::UnsignedInteger bx = params.get<xdata::UnsignedInteger>("bx");
  xdata::UnsignedInteger cycle = params.get<xdata::UnsignedInteger>("cycle");
  
  // FIXME: Embed this check somewhere
  ::mp7::MP7Controller& driver = getParent< swatch::mp7::MP7Processor>()->driver();
  ::mp7::orbit::Metric metric = driver.getMetric();

 if ( !orbit::isValid( bx, cycle, metric ) ) {
    std::ostringstream msg;
    msg << "Invalid orbit point parameters (" << bx.value_ << ", " << cycle.value_ << ")";

    setStatusMsg(msg.str());
    return kError;
  }
  //--
    
  ::mp7::ChannelsManager cm = getChannelsMgr(params);

  std::ostringstream alTo;
  alTo << "Aligning the MP MGTs to " << bx << ", " << cycle << "...";
  setProgress(0., alTo.str());


  try {
    ::mp7::orbit::Point p = ::mp7::orbit::Point(bx, cycle);
    cm.alignLinks(p);
  }  catch (std::exception &e) {
    std::ostringstream err;
    err << "Exception caught while trying to align: " << e.what();
    setStatusMsg(err.str());
    return kError;
  }

  setProgress(0.6, "Checking Links...");
  try {
    cm.checkLinks();
  }  catch (std::exception &e) {
    std::ostringstream err;
    err << "Exception caught while trying to check links: " << e.what();
    setStatusMsg(err.str());
    return kError;
  }

  setStatusMsg("Align MP MGTs completed");
  return kDone;
}


// --------------------------------------------------------
AutoAlignCommand::AutoAlignCommand(const std::string& aId) :
  AbstractChannelsCommand(aId, AbstractChannelsCommand::kRx, xdata::String())
{
  registerParameter("margin", xdata::UnsignedInteger(3));
}


// --------------------------------------------------------
AutoAlignCommand::~AutoAlignCommand() {}


// --------------------------------------------------------
core::Command::State AutoAlignCommand::code(const core::XParameterSet& aParams)
{
  xdata::UnsignedInteger lMargin = aParams.get<xdata::UnsignedInteger>("margin");
  
  ::mp7::ChannelsManager lChanMgr = getChannelsMgr(aParams);
  
  setProgress(0.01, "Starting auto-align with margin="+boost::lexical_cast<std::string>(lMargin.value_));
  
  lChanMgr.minimizeAndAlignLinks(lMargin.value_);
  
  return kDone;
}


} //end ns mp7
} //end ns swatch