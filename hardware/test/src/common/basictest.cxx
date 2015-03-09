/* 
 * File:   basictest.cxx
 * Author: ale
 *
 * Created on November 2, 2014, 10:32 PM
 */

#include <cstdlib>

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/XParameterSet.hpp"
#include "swatch/core/xoperators.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/processor/ProcessorFactory.hpp"
#include "swatch/system/Crate.hpp"

// Swatch Hardware Headers
#include "swatch/hardware/MP7Processor.hpp"
#include "swatch/hardware/MP7Controls.hpp"
#include "swatch/hardware/MP7TTCInterface.hpp"
#include "swatch/hardware/AMC13Service.hpp"

// uHAL Headers
#include "uhal/log/log.hpp"
#include "swatch/system/SystemFactory.hpp"
#include "swatch/system/ServiceFactory.hpp"
#include "swatch/system/Utilities.hpp"

// XDAQ Headers
#include "xdata/Vector.h"
#include <xdata/Integer.h>

// Boost Headers
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>

// AMC13 Headers
#include "amc13/AMC13.hh"
#include "swatch/core/Port.hpp"

using namespace std;
namespace swco = swatch::core;
namespace swlo = swatch::logger;
namespace swhw = swatch::hardware;
namespace swsys = swatch::system;
namespace swpro = swatch::processor;

class ResetClockCommand : public swco::Command {
public:
  ResetClockCommand(swco::ActionHandler* aHandler) :
    Command(aHandler, xdata::Integer()) {
  }
  
  virtual ~ResetClockCommand() {
  }


  virtual void code() {
    
    swsys::System* sys = getHandler<swsys::System>();
    
    LOG(swlo::kNotice) << "Resetting AMC13s";

    BOOST_FOREACH( swsys::System::CratesMap::value_type& p, sys->getCrates()) {
        swsys::Crate* crate = p.second;

        crate->amc13()->reset();
    }
      
    LOG(swlo::kNotice) << "Resetting AMC13 TTC";

    BOOST_FOREACH( swsys::System::CratesMap::value_type& p, sys->getCrates()) {
        swsys::Crate* crate = p.second;

        crate->amc13()->configureClock("ttsloopback");
        
    }
    
    
    // Loop over the crates to configure the AMC13s
    BOOST_FOREACH( swsys::System::CratesMap::value_type& p, sys->getCrates()) {
        swsys::Crate* crate = p.second;

        LOG(swlo::kInfo) << "Crate: "  << p.first << " - addr :" << crate;

        swhw::AMC13Service* amc13 = dynamic_cast<swhw::AMC13Service*>(crate->amc13());
        LOG(swlo::kInfo) << "populated slots: " << swlo::shortVecFmt(crate->getPopulatedSlots());

        // 
        std::vector<uint32_t> slots = crate->getAMCSlots();
        LOG(swlo::kInfo) << "Enabling slots " << swlo::shortVecFmt(slots);
        amc13->enableTTC(slots);

        amc13->driver()->getStatus()->Report(3);
      
    }
     
    
    setProgress(50., "AMC13 configured");

    // Loop over the crates to configure the AMC13s
    BOOST_FOREACH( swsys::System::CratesMap::value_type& p, sys->getCrates()) {
        swsys::Crate* crate = p.second;

        std::vector<uint32_t> slots = crate->getAMCSlots();

        BOOST_FOREACH( uint32_t s, crate->getAMCSlots()) {
          swpro::Processor* p = crate->amc(s);
          
          p->getCommand("reset")->exec();
          
          LOG(swlo::kInfo) << "BC0 Locked" << p->ttc()->isOrbitLocked();
        }

    }
    setDone("AMC13 & MP7(s) reset and locked on TTC Clock/BC0");

  }

};


/*
 * 
 */
int main(int argc, char** argv) {
    using boost::property_tree::ptree;
    using boost::property_tree::json_parser::read_json;
    using swco::XParameterSet;
    using swco::shellExpandPath;

    uhal::setLogLevelTo(uhal::Warning());
    
    // Build the property tree
    ptree pt;
    read_json(shellExpandPath("${SWATCH_ROOT}/hardware/test/cfg/firstsys.json"), pt);
    XParameterSet sysset = swsys::treeToSystemPars(pt);
    
    LOG(swlo::kNotice) << "Building the new system";
    swsys::System* mysys = swsys::SystemFactory::get()->make("SystemCreator", sysset.get<xdata::String>("name"), sysset);
    mysys->registerCommand<ResetClockCommand>("resetClocks");
    
    mysys->getCommand("resetClocks")->exec();
    
    BOOST_FOREACH( swpro::Processor* p, mysys->getProcessors() ) {
      p->getCommand("loopback")->exec();
      
      LOG(swlo::kInfo) << left 
                       << setw(15) << "id" << " | "
                       << setw(7) << "enabled" <<  " | " 
                       << setw(9) << "operating" << " | "
                       << setw(7) << "aligned" << " | "
                       << setw(6) << "crcs" ;
      BOOST_FOREACH( swco::InputPort* ip, p->getInputs() ){
        LOG(swlo::kInfo) << left 
            << setw(15) << ip->path() << " | "
            << setw(7) << ip->isEnabled() << " | "
            << setw(9) << ip->isLocked() << " | "
            << setw(7) << ip->isAligned() << " | "
            << setw(6) << ip->getCRCErrors() ;
      } 
    }

    return 0;
}

