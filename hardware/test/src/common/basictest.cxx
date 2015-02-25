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

// MP7 Processor
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

// Boost Headers
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "amc13/AMC13.hh"

using namespace std;
namespace swco = swatch::core;
namespace swlo = swatch::logger;
namespace swhw = swatch::hardware;
namespace swsys = swatch::system;
namespace swpro = swatch::processor;
/*
 * 
 */
int main(int argc, char** argv) {
    using boost::property_tree::ptree;
    using boost::property_tree::json_parser::read_json;
    using swco::XParameterSet;
    using swco::shellExpandPath;

    uhal::setLogLevelTo(uhal::Warning());
    
//    exit(0);
    
    // Build the property tree
    ptree pt;
    read_json(shellExpandPath("${SWATCH_ROOT}/hardware/test/cfg/firstsys.json"), pt);
    XParameterSet sysset = swsys::treeToSystemPars(pt);
    
    LOG(swlo::kNotice) << "Building the new system";
    swsys::System* mysys = swsys::SystemFactory::get()->make("SystemCreator", sysset.get<xdata::String>("name"), sysset);
    
    LOG(swlo::kNotice) << "Resetting TTC";

    // Loop over the crates to configure the AMC13s
    BOOST_FOREACH( swsys::System::CratesMap::value_type& p, mysys->getCrates()) {
        swsys::Crate* crate = p.second;

        LOG(swlo::kInfo) << "Crate: "  << p.first << " - addr :" << crate;

        swhw::AMC13Service* amc13 = dynamic_cast<swhw::AMC13Service*>(crate->amc13());
        LOG(swlo::kInfo) << "populated slots: " << swlo::shortVecFmt(crate->getPopulatedSlots());

        swco::Command* reset = amc13->getCommand("reset");
        reset->getParameters().set("mode", xdata::String("ttsloopback"));

        LOG(swlo::kInfo) << " -- AMC13 reset parameters";

        BOOST_FOREACH( const std::string& k, reset->getParameters().keys()) {
          LOG(swlo::kInfo) << k << " : " << reset->getParameters()[k];
        }

        reset->exec();

        // 
        std::vector<uint32_t> slots = crate->getAMCSlots();
        LOG(swlo::kInfo) << "Enabling slots " << swlo::shortVecFmt(slots);
        amc13->enableTTC(slots);

        amc13->driver()->getStatus()->Report(3);
      
    }
    

    // Loop over the crates to configure the AMC13s
    BOOST_FOREACH( swsys::System::CratesMap::value_type& p, mysys->getCrates()) {
        swsys::Crate* crate = p.second;

        std::vector<uint32_t> slots = crate->getAMCSlots();

        BOOST_FOREACH( uint32_t s, crate->getAMCSlots()) {
          swpro::Processor* p = crate->amc(s);
          
          p->getCommand("reset")->exec();
        }

    }

    /*
    exit(0);

    // TODO: Add amc13 reset
    swco::Command* reset = amc13_A->getCommand("reset");
    reset->getParameters().set("mode", xdata::String("ttsloopback"));

    LOG(swlo::kNotice) << " -- AMC13 reset parameters";

    BOOST_FOREACH( const std::string& k, reset->getParameters().keys()) {
        LOG(swlo::kNotice) << k << " : " << reset->getParameters()[k];
    }
    reset->exec();
    // TODO: add a check that the communication is established
    

    // Mimic what System configure will need to do
    std::vector<uint32_t> slots;
    // enable slot 10
    slots.push_back(mp7_A->getSlot());
    amc13_A->enableTTC(slots);

    // exit(0);
    

    
//    BOOST_FOREACH( const std::string& mode, mp7_A->getModes() ) {
//        LOG(swlo::kNotice) << " - " << mode;
//    }
    
//    exit(0);
//    mp7_A->reset("external");

    // mp7_A->reset("internal");

    mp7_A->getCommand("reset")->exec();
    
    mp7_A->ttc()->clearCounters();
    
    LOG(swlo::kNotice) << "Checking TTC Status";
    LOG(swlo::kNotice) << "clock 40 locked:   " << mp7_A->ttc()->isClock40Locked();
    LOG(swlo::kNotice) << "clock 40 error:   " << mp7_A->ttc()->hasClock40Stopped();
    LOG(swlo::kNotice) << "bc0 locked:   " << mp7_A->ttc()->isOrbitLocked();
    LOG(swlo::kNotice) << "bc0 error:   " << mp7_A->ttc()->hasBC0Stopped();
    
    LOG(swlo::kNotice) << "bx count:   " << mp7_A->ttc()->getBunchCounter();
    LOG(swlo::kNotice) << "orb count:  " << mp7_A->ttc()->getOrbitCounter();
    LOG(swlo::kNotice) << "ev count:   " << mp7_A->ttc()->getEventCounter();
    LOG(swlo::kNotice) << "sbe counts: " << mp7_A->ttc()->getSingleBitErrors();
    LOG(swlo::kNotice) << "sbe counts: " << mp7_A->ttc()->getDoubleBitErrors();
    */
    return 0;
}

