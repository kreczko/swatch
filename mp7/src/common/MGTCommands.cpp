
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

#include "swatch/mp7/MP7AbstractProcessor.hpp"
#include "swatch/core/toolbox/IntListParser.hpp"
#include "swatch/mp7/Utilities.hpp"

namespace swatch {
namespace mp7 {


// --------------------------------------------------------


ConfigureRxMGTsCommand::ConfigureRxMGTsCommand(const std::string& aId, swatch::core::ActionableObject& aActionable) :
swatch::core::Command(aId, aActionable, xdata::String()),
mRxMGTHelper(*this)
{

  mRxMGTHelper.addCommandParameters();
  registerParameter("orbitTag", xdata::Boolean(false));
  registerParameter("polarity", xdata::Boolean(true));

}


// --------------------------------------------------------


swatch::core::Command::State
ConfigureRxMGTsCommand::code(const swatch::core::XParameterSet& params)
{
  bool orbitTag = params.get<xdata::Boolean>("orbitTag").value_;
  bool polarity = params.get<xdata::Boolean>("polarity").value_;

  ::mp7::ChannelManager cm = mRxMGTHelper.manager(params);

  setProgress(0.0, "Configuring Rx MGTs...");

  try {
    cm.configureRxMGTs(orbitTag, polarity);
  } catch (std::exception& e) {
    std::ostringstream err;
    err << "Exception caught while trying to configure Rx MGTs: " << e.what();
    setStatusMsg(err.str());
    return State::kError;
  }

  setStatusMsg("Configure Rx MGTs complete");
  return State::kDone;
}

// --------------------------------------------------------


ConfigureTxMGTsCommand::ConfigureTxMGTsCommand(const std::string& aId, swatch::core::ActionableObject& aActionable) :
swatch::core::Command(aId, aActionable, xdata::String()),
mTxMGTHelper(*this)
{

  mTxMGTHelper.addCommandParameters();

  registerParameter("orbitTag", xdata::Boolean(false));
  registerParameter("loopback", xdata::Boolean(false));
  registerParameter("polarity", xdata::Boolean(true));

}

// --------------------------------------------------------


swatch::core::Command::State
ConfigureTxMGTsCommand::code(const swatch::core::XParameterSet& params)
{
  bool orbitTag = params.get<xdata::Boolean>("orbitTag").value_;
  bool loopback = params.get<xdata::Boolean>("loopback").value_;
  bool polarity = params.get<xdata::Boolean>("polarity").value_;

  ::mp7::ChannelManager cm = mTxMGTHelper.manager(params);

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
  } catch (std::exception& e) {
    std::ostringstream err;
    err << "Exception caught while trying to configure Tx MGTs: " << e.what();
    setStatusMsg(err.str());
    return State::kError;
  }

  setStatusMsg("Configure Tx MGTs completed");
  return State::kDone;
}


// --------------------------------------------------------


AlignRxsToCommand::AlignRxsToCommand(const std::string& aId, swatch::core::ActionableObject& aActionable) :
swatch::core::Command(aId, aActionable, xdata::String()),
mRxMGTHelper(*this)
{

  mRxMGTHelper.addCommandParameters();

  registerParameter("bx", xdata::UnsignedInteger(0x0));
  registerParameter("cycle", xdata::UnsignedInteger(0x0));
}


// --------------------------------------------------------


core::Command::State
AlignRxsToCommand::code(const swatch::core::XParameterSet& params)
{

  xdata::UnsignedInteger bx = params.get<xdata::UnsignedInteger>("bx");
  xdata::UnsignedInteger cycle = params.get<xdata::UnsignedInteger>("cycle");

  // FIXME: Embed this check somewhere
  ::mp7::MP7MiniController& driver = getActionable<MP7AbstractProcessor>().driver();
  ::mp7::orbit::Metric metric = driver.getMetric();

  if (!orbit::isValid(bx, cycle, metric)) {
    std::ostringstream msg;
    msg << "Invalid orbit point parameters (" << bx.value_ << ", " << cycle.value_ << ")";

    setStatusMsg(msg.str());
    return State::kError;
  }
  //--

  ::mp7::ChannelManager cm = mRxMGTHelper.manager(params);

  std::ostringstream alTo;
  alTo << "Aligning the MP MGTs to " << bx << ", " << cycle << "...";
  setProgress(0., alTo.str());


  try {
    ::mp7::orbit::Point p = ::mp7::orbit::Point(bx, cycle);
    cm.alignLinks(p);
  } catch (std::exception &e) {
    std::ostringstream err;
    err << "Exception caught while trying to align: " << e.what();
    setStatusMsg(err.str());
    return State::kError;
  }

  setProgress(0.6, "Checking Links...");
  try {
    cm.checkMGTs();
  } catch (std::exception &e) {
    std::ostringstream err;
    err << "Exception caught while trying to check links: " << e.what();
    setStatusMsg(err.str());
    return State::kError;
  }

  setStatusMsg("Align MP MGTs completed");
  return State::kDone;
}


// --------------------------------------------------------


AutoAlignCommand::AutoAlignCommand(const std::string& aId, swatch::core::ActionableObject& aActionable) :
swatch::core::Command(aId, aActionable, xdata::String()),
mRxMGTHelper(*this)
{
  mRxMGTHelper.addCommandParameters();

  registerParameter("margin", xdata::UnsignedInteger(3));
}


// --------------------------------------------------------


core::Command::State AutoAlignCommand::code(const core::XParameterSet& aParams)
{
  xdata::UnsignedInteger lMargin = aParams.get<xdata::UnsignedInteger>("margin");

  ::mp7::ChannelManager lChanMgr = mRxMGTHelper.manager(aParams);

  setProgress(0.01, "Starting auto-align with margin=" + boost::lexical_cast<std::string>(lMargin.value_));

  lChanMgr.minimizeAndAlignLinks(lMargin.value_);

  return State::kDone;
}


} //end ns mp7
} //end ns swatch
