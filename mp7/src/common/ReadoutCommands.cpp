/* 
 * File:   Readout.cpp
 * Author: ale
 * 
 * Created on October 2, 2015, 4:56 PM
 */

#include "swatch/mp7/ReadoutCommands.hpp"


// MP7 Headers
#include "mp7/CtrlNode.hpp"
#include "mp7/MP7Controller.hpp"
#include "mp7/ReadoutCtrlNode.hpp"
#include "mp7/ReadoutNode.hpp"
#include "mp7/TTCNode.hpp"

// XDAQ Headers
#include "xdata/Boolean.h"
#include "xdata/String.h"
#include "xdata/UnsignedInteger.h"

// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/mp7/MP7AbstractProcessor.hpp"


namespace swatch {
namespace mp7 {


SetupReadout::SetupReadout( const std::string& aId, swatch::core::ActionableObject& aActionable ) :
  core::Command(aId, aActionable, xdata::UnsignedInteger())
{
  registerParameter("internal", xdata::Boolean(false));
  registerParameter("bxOffset", xdata::UnsignedInteger(1));
  registerParameter("drain", xdata::UnsignedInteger());
  registerParameter("bufferHWM", xdata::UnsignedInteger(32));
  registerParameter("bufferLWM", xdata::UnsignedInteger(16));
  registerParameter("fake", xdata::UnsignedInteger());
}


SetupReadout::~SetupReadout() {
}


core::Command::State 
SetupReadout::code(const ::swatch::core::XParameterSet& aParams)
{
  bool lInternal = aParams.get<xdata::Boolean>("internal").value_;
  uint32_t lBxOffset = aParams.get<xdata::UnsignedInteger>("bxOffset").value_;
  uint32_t lBufferHWM = aParams.get<xdata::UnsignedInteger>("bufferHWM").value_;
  uint32_t lBufferLWM = aParams.get<xdata::UnsignedInteger>("bufferLWM").value_;

  xdata::UnsignedInteger lDrain = aParams.get<xdata::UnsignedInteger>("drain");
  xdata::UnsignedInteger lFake = aParams.get<xdata::UnsignedInteger>("fake");

  MP7AbstractProcessor& p = getActionable<MP7AbstractProcessor>();
  ::mp7::MP7Controller& driver = p.driver();

  const ::mp7::TTCNode& ttc = driver.getTTC();
  const ::mp7::ReadoutNode& ro = driver.getReadout();
  const ::mp7::ReadoutCtrlNode& rc = ro.getNode("readout_control");

  uint32_t roSize = driver.getGenerics().roChunks * 2;
  if ( lBufferLWM >= roSize or lBufferHWM >= roSize) {
    std::ostringstream msg;
    msg << "RO buffer Watermarks higher than its size : lwm = %d, hwm - %d, rosize = %d";
    setStatusMsg(msg.str());
    return State::kError;
  }

  // Enable trigger rules if in internal mode
  ttc.enableL1ATrgRules(lInternal);
  
  // Enable trigger throttling
  ttc.enableL1AThrottling(lInternal);

  // Set the readout counter offset
  ro.setBxOffset(lBxOffset);

  //  set TTS status by hand.
  //  1 = warningt
  //  2 = out of sync
  //  4 = busy
  //  8 = ready
  //  12 = error
  //  0 or 15 = disconnected
  ro.forceTTSState(false);

  // To drain or not to drain in internal mode?
  if ( lDrain.isNaN()) {
      LOG(swatch::logger::kInfo) << "Autodrain : disabled";
      ro.enableAutoDrain(false);
  } else {
      LOG(swatch::logger::kInfo) << "Autodrain rate : 0x" << std::hex << lDrain.value_;
      ro.enableAutoDrain(true, lDrain.value_);
  }


  // Configure big fifo's watermarks
  // Maximum: 64x
  // High water mark : 32 - 50%
  // Low water mark : 16 - 25%
  ro.setFifoWaterMarks(lBufferLWM, lBufferHWM);

  // And the derandomisers
  // Maximum: 512
  // TODO: check with Fionn
  rc.setDerandWaterMarks(64,128);


  if (lFake.isNaN()) {
      // logging.info("ReadoutControl event source selected")
      ro.selectEventSource( ::mp7::ReadoutNode::kReadoutEventSource );
  } else {
      // logging.info("Fake event source selected, event size : 0x%x", fakesize)
      ro.selectEventSource( ::mp7::ReadoutNode::kFakeEventSource );
      ro.configureFakeEventSize(lFake.value_);
  }

  // declare the board ready for readout
  ro.start();

  // Local mode, amc13 link disabled
  LOG(swatch::logger::kInfo) << "AMC13 output : "  << (not lInternal ? "enabled" : "disabled");
  ro.enableAMC13Output(not lInternal);
  
  return State::kDone;
}


// --------------------------------------------------------
LoadReadoutMenu::LoadReadoutMenu( const std::string& aId, swatch::core::ActionableObject& aActionable ) :
  swatch::core::Command(aId, aActionable, xdata::UnsignedInteger())
{
  
	::mp7::MP7Controller& lDriver = getActionable<MP7AbstractProcessor>().driver();
	const ::mp7::ReadoutCtrlNode& rc = lDriver.getReadout().getNode< ::mp7::ReadoutCtrlNode>("readout_control");

	mBanks = rc.readNumBanks();
	mModes = rc.readNumModes();
	mCaptures = rc.readNumCaptures();
  
  std::string bankStr, modeStr, capStr;
  for( uint32_t iB(0); iB < mBanks; ++iB ) {
    bankStr = "bank"+boost::lexical_cast<std::string>(iB)+":";
    registerParameter(bankStr+"wordsPerBx", xdata::UnsignedInteger());
  }
  

  for( uint32_t iM(0); iM < mModes; ++iM ) {

      // <entry id="roMenu.mode1:eventSize" type="uint"></entry>
      // <entry id="roMenu.mode1:eventsToTrigger" type="uint"></entry>
      // <entry id="roMenu.mode1:eventType" type="uint"></entry>
      // <entry id="roMenu.mode1:tokenDelay" type="uint"></entry>

    modeStr = "mode"+boost::lexical_cast<std::string>(iM)+":";
    registerParameter(modeStr+"eventSize", xdata::UnsignedInteger());
    registerParameter(modeStr+"eventToTrigger", xdata::UnsignedInteger());
    registerParameter(modeStr+"eventType", xdata::UnsignedInteger());
    registerParameter(modeStr+"tokenDelay", xdata::UnsignedInteger());

    for( uint32_t iC(0); iC < mCaptures; ++iC ) {

      // <entry id="roMenu.mode0:capture0:enable" type="bool"></entry>
      // <entry id="roMenu.mode0:capture0:id" type="uint"></entry>
      // <entry id="roMenu.mode0:capture0:bankId" type="uint"></entry>
      // <entry id="roMenu.mode0:capture0:length" type="uint"></entry>
      // <entry id="roMenu.mode0:capture0:delay" type="uint"></entry>
      // <entry id="roMenu.mode0:capture0:readoutLength" type="uint"></entry>

      capStr = modeStr+"capture"+boost::lexical_cast<std::string>(iC)+":";

      registerParameter(capStr+"enable", xdata::Boolean());
      registerParameter(capStr+"id", xdata::UnsignedInteger());
      registerParameter(capStr+"bankId", xdata::UnsignedInteger());
      registerParameter(capStr+"length", xdata::UnsignedInteger());
      registerParameter(capStr+"delay", xdata::UnsignedInteger());
      registerParameter(capStr+"readoutLength", xdata::UnsignedInteger());

    }
  }
}


// --------------------------------------------------------
LoadReadoutMenu::~LoadReadoutMenu() {
}

// --------------------------------------------------------
//LoadReadoutMenu*
//LoadReadoutMenu::create( const std::string& aId, ::mp7::MP7Controller& aDriver) {
//  const ::mp7::ReadoutCtrlNode& rc = aDriver.getReadout().getNode< ::mp7::ReadoutCtrlNode >("readout_control");
//  uint32_t lBanks = rc.readNumBanks();
//  uint32_t lModes = rc.readNumModes();
//  uint32_t lCaptures = rc.readNumCaptures();
//
//  return new LoadReadoutMenu(aId, aDaDriver, lBanks, lModes, lCaptures);
//}

// --------------------------------------------------------
core::Command::State
LoadReadoutMenu::code(const ::swatch::core::XParameterSet& aParams)
{

  ::mp7::ReadoutMenu lMenu(mBanks, mModes, mCaptures);

  // Bank IDs
  std::string bankStr, modeStr, capStr;
  for( uint32_t iB(0); iB < mBanks; ++iB ) {
    bankStr = "bank"+boost::lexical_cast<std::string>(iB)+":";
    const xdata::UnsignedInteger& bxOffset = aParams.get<xdata::UnsignedInteger>(bankStr+"wordsPerBx");
    if ( !bxOffset.isNaN()) lMenu.bank(iB).wordsPerBx = bxOffset.value_;

  }
  
  for( uint32_t iM(0); iM < mModes; ++iM ) {

      // <entry id="roMenu.mode1:eventSize" type="uint"></entry>
      // <entry id="roMenu.mode1:eventsToTrigger" type="uint"></entry>
      // <entry id="roMenu.mode1:eventType" type="uint"></entry>
      // <entry id="roMenu.mode1:tokenDelay" type="uint"></entry>

    modeStr = "mode"+boost::lexical_cast<std::string>(iM)+":";
    const xdata::UnsignedInteger& eventSize = aParams.get<xdata::UnsignedInteger>(modeStr+"eventSize");
    const xdata::UnsignedInteger& eventToTrigger = aParams.get<xdata::UnsignedInteger>(modeStr+"eventToTrigger");
    const xdata::UnsignedInteger& eventType = aParams.get<xdata::UnsignedInteger>(modeStr+"eventType");
    const xdata::UnsignedInteger& tokenDelay = aParams.get<xdata::UnsignedInteger>(modeStr+"tokenDelay");

    ::mp7::ReadoutMenu::Mode& lMode = lMenu.mode(iM);
    if ( !eventSize.isNaN()) lMode.eventSize = eventSize.value_;
    if ( !eventToTrigger.isNaN()) lMode.eventToTrigger = eventToTrigger.value_;
    if ( !eventType.isNaN()) lMode.eventType = eventType.value_;
    if ( !tokenDelay.isNaN()) lMode.tokenDelay = tokenDelay.value_;

    LOG(swatch::logger::kWarning) << lMode;


    for( uint32_t iC(0); iC < mCaptures; ++iC ) {
      capStr = modeStr+"capture"+boost::lexical_cast<std::string>(iC)+":";

      const xdata::Boolean& enable = aParams.get<xdata::Boolean>(capStr+"enable");
      const xdata::UnsignedInteger& id = aParams.get<xdata::UnsignedInteger>(capStr+"id");
      const xdata::UnsignedInteger& bankId = aParams.get<xdata::UnsignedInteger>(capStr+"bankId");
      const xdata::UnsignedInteger& length = aParams.get<xdata::UnsignedInteger>(capStr+"length");
      const xdata::UnsignedInteger& delay = aParams.get<xdata::UnsignedInteger>(capStr+"delay");
      const xdata::UnsignedInteger& readoutLength = aParams.get<xdata::UnsignedInteger>(capStr+"readoutLength");
      
      ::mp7::ReadoutMenu::Capture& lCapture = lMode[iC];
      
      if ( !enable.isNaN()) lCapture.enable = enable.value_;
      if ( !id.isNaN()) lCapture.id = id.value_;
      if ( !bankId.isNaN()) lCapture.bankId = bankId.value_;
      if ( !length.isNaN()) lCapture.length = length.value_;
      if ( !delay.isNaN()) lCapture.delay = delay.value_;
      if ( !readoutLength.isNaN()) lCapture.readoutLength = readoutLength.value_;
       
    }
  }
  
  ::mp7::MP7Controller& driver = getActionable<MP7AbstractProcessor>().driver();
  const ::mp7::ReadoutCtrlNode& rc = driver.getReadout().getNode< ::mp7::ReadoutCtrlNode >("readout_control"); 
  
  
  std::map<uint32_t,uint32_t> lEventSizes = driver.computeEventSizes(lMenu);
  for( uint32_t iM(0); iM < mModes; ++iM ) {
    ::mp7::ReadoutMenu::Mode& lMode = lMenu.mode(iM);
    if ( lMode.eventSize == 0xfffff ) continue;
    lMode.eventSize = lEventSizes.at(iM);
    LOG(swatch::logger::kInfo) << "Mode " << iM << " event size set to " << lMode.eventSize;
  }
  
  LOG(swatch::logger::kInfo) << lMenu;
  

  rc.configureMenu(lMenu);

  return State::kDone;
}


} //end ns mp7
} //end ns swatch
