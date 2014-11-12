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
using namespace swatch::core;

/*
 * 
 */
int main(int argc, char** argv) {
    using boost::property_tree::ptree;
    using boost::property_tree::json_parser::read_json;
    using swatch::core::ParameterSet;
    using swatch::core::shellExpandPath;

    uhal::setLogLevelTo(uhal::Warning());

    LOG(swatch::logger::kError) << "sticazzi";
    LOG(swatch::logger::kWarning) << "sticazzi";
    LOG(swatch::logger::kInfo) << "sticazzi";
    LOG(swatch::logger::kDebug) << "sticazzi";
    LOG(swatch::logger::kDebug1) << "sticazzi";
    
    exit(0);
    
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
        std::cout << "ERROR: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Crap..." << std::endl;
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
        std::cout << "ERROR: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Crap..." << std::endl;
    }
    
    BOOST_FOREACH( const std::string& mode, mp7_A->getModes() ) {
        std::cout << " - " << mode << std::endl;
    }
    
//    exit(0);
    mp7_A->reset("external");

    // mp7_A->reset("internal");
    
    mp7_A->ttc()->clearCounters();
    
    std::cout << "Checking TTC Status" << std::endl;
    std::cout << "clock 40 locked:   " << mp7_A->ttc()->isClock40Locked() << std::endl;
    std::cout << "clock 40 error:   " << mp7_A->ttc()->hasClock40Stopped() << std::endl;
    std::cout << "bc0 locked:   " << mp7_A->ttc()->isOrbitLocked() << std::endl;
    std::cout << "bc0 error:   " << mp7_A->ttc()->hasBC0Stopped() << std::endl;
    
    std::cout << "bx count:   " << mp7_A->ttc()->getBunchCounter() << std::endl;
    std::cout << "orb count:  " << mp7_A->ttc()->getOrbitCounter() << std::endl;
    std::cout << "ev count:   " << mp7_A->ttc()->getEventCounter() << std::endl;
    std::cout << "sbe counts: " << mp7_A->ttc()->getSingleBitErrors() << std::endl;
    std::cout << "sbe counts: " << mp7_A->ttc()->getDoubleBitErrors() << std::endl;

    return 0;
}

