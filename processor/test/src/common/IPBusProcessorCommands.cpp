/**
 * @file    IPBusProcessor.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */


#include <xdata/Integer.h>

// Swatch Headers
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
namespace swco = swatch::core;

namespace swatch {
namespace processor {
namespace test {


//---
IPBusResetCommand::IPBusResetCommand(core::ActionHandler* aHandler) :
  Command(aHandler, xdata::Integer()) {
  
  getParams().add("mode",xdata::String());
  

}


//---
IPBusResetCommand::~IPBusResetCommand() {
}


//---
void IPBusResetCommand::code() {

    IPBusProcessor* p = getHandler<IPBusProcessor>();
    
    if ( !getParams().has("mode") ) {
      setError("Configuration not found!");
      return;
    }
    
    std::string config = getParams().get<xdata::String>("mode");

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
      setError("Unknown configuration mode '"+config+"'");

      return;
    }

    p->hw().getNode("ttc.stat.bc0Locked").write(true);
    p->hw().dispatch();


    p->ttc()->clearErrors();
    p->ttc()->clearCounters();
    
    setDone("Done!");
}



//---
IPBusConfigureCommand::IPBusConfigureCommand(core::ActionHandler* aHandler) :
  Command(aHandler, xdata::String()) {
  
    getParams().add("mode",xdata::String());
}

IPBusConfigureCommand::~IPBusConfigureCommand() {
  
}

void IPBusConfigureCommand::code() {
  
    using namespace swatch::core;
    if ( !getParams().has("mode") ) {
      setError("Configuration not found!");
      return;
    }
    
    std::string config = getParams().get<xdata::String>("mode");

    IPBusProcessor* p = getHandler<IPBusProcessor>();

    // // Check presence of TTC clock and signals
     if ( not p->ttc()->isClock40Locked() ) {
         LOG(swlog::kError) << "Clock 40 NOT locked!";
     }
     if ( not p->ttc()->isOrbitLocked() ) {
         LOG(swlog::kError) << "Orbit (BC0) NOT locked!";
     }

    // // And clear all counters
    // p->ttc()->clearCounters();
    
   
    if ( config == "capture") {
      // And then buffers
      BOOST_FOREACH(swco::InputPort* in, p->getInputs() ) {
          dynamic_cast<IPBusRxChannel*>(in)->configureBuffer(BufferInterface::Capture);
      }
      BOOST_FOREACH( swco::OutputPort* out, p->getOutputs() ) {
          dynamic_cast<IPBusTxChannel*>(out)->configureBuffer(BufferInterface::Capture);
      }
    } else {
      setError("Unknown configuration mode '"+config+"'");
      return;
    }


    // Configure board DAQ block
    

    // Configure board’s algorithm block with scalers and LUTs
    

    // Post-configuration checks (if needed)
    

    // Check links alignment (?)
}

IPBusCapture::IPBusCapture(core::ActionHandler* aHandler) :
  Command(aHandler, xdata::String()) {
}

IPBusCapture::~IPBusCapture() {
}

void IPBusCapture::code() {

    IPBusProcessor* p = getHandler<IPBusProcessor>();

    LOG(swlog::kNotice) << ">> Sending test BGo";
    p->ttc()->sendBGo(0xc);

    LOG(swlog::kNotice) << ">> Take a nap (1 sec)";
    sleep(1);
    
    std::vector< std::vector<uint64_t> > data;
    data.reserve(p->getNumOutputs());
    BOOST_FOREACH(swco::OutputPort* out, p->getOutputs() ) {
      LOG(swlog::kDebug) << "Downloading " << out->id();
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
}




} // namespace test
} // namespace processor
} // namespace swatch

