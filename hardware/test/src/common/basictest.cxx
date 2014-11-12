/* 
 * File:   basictest.cxx
 * Author: ale
 *
 * Created on November 2, 2014, 10:32 PM
 */

#include <cstdlib>

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/ParameterSet.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/processor/ProcessorFactory.hpp"

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

// Boost Headers
#include <boost/foreach.hpp>
 #include <boost/property_tree/json_parser.hpp>

using namespace std;
namespace swcore = swatch::core;
namespace swlog = swatch::logger;
namespace swhw = swatch::hardware;
/*
 * 
 */
int main(int argc, char** argv) {
    using boost::property_tree::ptree;
    using boost::property_tree::json_parser::read_json;
    using swcore::ParameterSet;
    using swcore::shellExpandPath;

    uhal::setLogLevelTo(uhal::Warning());

    LOG(swlog::kError) << "sticazzi";
    LOG(swlog::kWarning) << "sticazzi";
    LOG(swlog::kInfo) << "sticazzi";
    LOG(swlog::kDebug) << "sticazzi";
    LOG(swlog::kDebug1) << "sticazzi";
    
//    exit(0);
    
    // Build the property tree
    ptree pt;
    read_json(shellExpandPath("${SWATCH_ROOT}/hardware/test/cfg/firstsys.json"), pt);

    // And then turn it into parameters
    ParameterSet pset = swatch::system::treeToSystemPars(pt);
    ParameterSet amc13params = pset.get< std::deque<ParameterSet> >("services").front();
    ParameterSet mp7params = pset.get< std::deque<ParameterSet> >("processors").front();
    
    swatch::hardware::AMC13Service* amc13_A = 0x0;
    try {
        amc13_A = dynamic_cast<swatch::hardware::AMC13Service*>(swatch::system::ServiceFactory::get()->make(amc13params));
    } catch ( const std::exception& e ) {
        LOG(swlog::kError) << e.what();
    } catch (...) {
        LOG(swlog::kError) << "Crap...";
    }
    
    // TODO: Add amc13 reset
    amc13_A->reset("ttsloopback");
    // TODO: add a check that the communication is established
    
    // Mimic what System configure will need to do
    std::vector<uint32_t> slots;
    // enable slot 10
    slots.push_back(10);
    amc13_A->enableTTC(slots);

//     exit(0);
    
    // Create an MP7
    swatch::hardware::MP7Processor* mp7_A=0x0;
    try {
        mp7_A = dynamic_cast<swatch::hardware::MP7Processor*>(swatch::processor::ProcessorFactory::get()->make(mp7params));
    } catch ( const std::exception& e ) {
        LOG(swlog::kError) << e.what();
    } catch (...) {
        LOG(swlog::kError) << "Crap...";
    }
    
    BOOST_FOREACH( const std::string& mode, mp7_A->getModes() ) {
        LOG(swlog::kNotice) << " - " << mode;
    }
    
//    exit(0);
    mp7_A->reset("external");

    // mp7_A->reset("internal");
    
    mp7_A->ttc()->clearCounters();
    
    LOG(swlog::kNotice) << "Checking TTC Status";
    LOG(swlog::kNotice) << "clock 40 locked:   " << mp7_A->ttc()->isClock40Locked();
    LOG(swlog::kNotice) << "clock 40 error:   " << mp7_A->ttc()->hasClock40Stopped();
    LOG(swlog::kNotice) << "bc0 locked:   " << mp7_A->ttc()->isOrbitLocked();
    LOG(swlog::kNotice) << "bc0 error:   " << mp7_A->ttc()->hasBC0Stopped();
    
    LOG(swlog::kNotice) << "bx count:   " << mp7_A->ttc()->getBunchCounter();
    LOG(swlog::kNotice) << "orb count:  " << mp7_A->ttc()->getOrbitCounter();
    LOG(swlog::kNotice) << "ev count:   " << mp7_A->ttc()->getEventCounter();
    LOG(swlog::kNotice) << "sbe counts: " << mp7_A->ttc()->getSingleBitErrors();
    LOG(swlog::kNotice) << "sbe counts: " << mp7_A->ttc()->getDoubleBitErrors();

    return 0;
}

