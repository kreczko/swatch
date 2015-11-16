/**
 * @file    IPBusProcessor.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */


#include <xdata/Integer.h>

// Swatch Headers
#include "swatch/processor/PortCollection.hpp"
#include "swatch/processor/test/IPBusProcessorCommands.hpp"
#include "swatch/processor/test/IPBusProcessor.hpp"
#include "swatch/processor/test/IPBusTTC.hpp"
#include "swatch/processor/test/IPBusRxChannel.hpp"
#include "swatch/processor/test/IPBusTxChannel.hpp"
#include "swatch/logger/Log.hpp"

// XDAQ Headers
#include "xdata/String.h"

// Boost Headers
#include <boost/foreach.hpp>

namespace swlog = swatch::logger;
namespace swpro = swatch::processor;
// namespace swco = swatch::core;

namespace swatch {
namespace processor {
namespace test {


//---
IPBusResetCommand::IPBusResetCommand(const std::string& aId) :
  Command(aId, xdata::Integer()) {
  
  registerParameter("mode",xdata::String());
}


//---
IPBusResetCommand::~IPBusResetCommand() {
}


//---
core::Command::State IPBusResetCommand::code(const swatch::core::XParameterSet& params) {

    IPBusProcessor* p = getParent<IPBusProcessor>();
    
    if ( !getDefaultParams().has("mode") ) {
      setStatusMsg("Configuration not found!");
      return State::kError;
    }
    
    std::string config = getDefaultParams().get<xdata::String>("mode").value_;

    // p->ctrl()->configureClock(clock); 
    p->hw().getNode("ttc.stat.clk40Locked").write(true);
    p->hw().dispatch();

    if ( config == "internal" ) {
        // Disable ttc, enable internal generator
        p->hw().getNode("ttc.ctrl.enable").write(0x0);
        p->hw().getNode("ttc.ctrl.genBC0").write(0x1);
    } else if ( config == "extenal" ) {
        // Disable ttc, enable internal generator
        p->hw().getNode("ttc.ctrl.enable").write(0x0);
        p->hw().getNode("ttc.ctrl.genBC0").write(0x1); 
    } else {
      setStatusMsg("Unknown configuration mode '"+config+"'");
      return State::kError;
    }

    p->hw().getNode("ttc.stat.bc0Locked").write(true);
    p->hw().dispatch();


    p->getTTC().clearErrors();
    p->getTTC().clearCounters();
    
    setStatusMsg("Done!");
    return State::kDone;
}



//---
IPBusConfigureCommand::IPBusConfigureCommand(const std::string& aId) :
  Command(aId, xdata::String()) {
  
  registerParameter("mode",xdata::String());
}

IPBusConfigureCommand::~IPBusConfigureCommand() {
  
}

core::Command::State IPBusConfigureCommand::code(const swatch::core::XParameterSet& params) {
  
    using namespace swatch::core;
    if ( !getDefaultParams().has("mode") ) {
      setStatusMsg("Configuration not found!");
      return State::kError;
    }
    
    std::string config = getDefaultParams().get<xdata::String>("mode").value_;

    IPBusProcessor* p = getParent<IPBusProcessor>();

    // // Check presence of TTC clock and signals
/*     if ( not p->ttc()->isClock40Locked() ) {
         LOG(swlog::kError) << "Clock 40 NOT locked!";
     }
     if ( not p->ttc()->isBC0Locked() ) {
         LOG(swlog::kError) << "Orbit (BC0) NOT locked!";
     }
*/
    // // And clear all counters
    // p->ttc()->clearCounters();
    
   
    if ( config == "capture") {
      // And then buffers
      BOOST_FOREACH(InputPort* in, p->getPorts().getInputs() ) {
          dynamic_cast<IPBusRxChannel*>(in)->configureBuffer(BufferInterface::Capture);
      }
      BOOST_FOREACH( OutputPort* out, p->getPorts().getOutputs() ) {
          dynamic_cast<IPBusTxChannel*>(out)->configureBuffer(BufferInterface::Capture);
      }
    } else {
      setStatusMsg("Unknown configuration mode '"+config+"'");
      return State::kError;
    }


    // Configure board DAQ block
    

    // Configure board’s algorithm block with scalers and LUTs
    

    // Post-configuration checks (if needed)
    

    // Check links alignment (?)

    return State::kDone;
}

IPBusCapture::IPBusCapture(const std::string& aId) :
  Command(aId, xdata::String()) {
}

IPBusCapture::~IPBusCapture() {
}

core::Command::State IPBusCapture::code(const swatch::core::XParameterSet& params) {

    IPBusProcessor* p = getParent<IPBusProcessor>();

//    LOG(swlog::kNotice) << ">> Sending test BGo";
//    p->ttc()->sendBGo(0xc);
//
//    LOG(swlog::kNotice) << ">> Take a nap (1 sec)";
//    sleep(1);
    
    std::vector< std::vector<uint64_t> > data;
    data.reserve(p->getPorts().getNumOutputs());
    BOOST_FOREACH(OutputPort* out, p->getPorts().getOutputs() ) {
      LOG(swlog::kDebug) << "Downloading " << out->getId();
      IPBusTxChannel* tx = dynamic_cast<IPBusTxChannel*>(out);
      data.push_back(tx->download());
      
    }
    
    LOG(swlog::kDebug) << "data.size() " << data.size();

    for ( size_t j(0); j<data[0].size(); ++j ) {
        std::ostringstream msg;
        msg << "Frame " <<  std::dec << std::setfill('0') << std::setw(4) << j << " : ";
        for ( size_t k(0); k<data.size(); ++k )
            msg << "0x" << std::hex << std::setfill('0') << std::setw(8) << data[k][j] << "   ";
        LOG(swlog::kInfo) << msg.str();
    }
    
    return State::kDone;
}




} // namespace test
} // namespace processor
} // namespace swatch

