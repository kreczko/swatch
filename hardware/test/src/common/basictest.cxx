/* 
 * File:   basictest.cxx
 * Author: ale
 *
 * Created on November 2, 2014, 10:32 PM
 */

#include <cstdlib>



// Swatch Headers
#include "swatch/core/ParameterSet.hpp"

// MP7 Processor
#include "swatch/hardware/MP7Processor.hpp"
#include "swatch/hardware/MP7Controls.hpp"
#include "swatch/hardware/MP7TTCInterface.hpp"
#include "swatch/hardware/AMC13Service.hpp"

// uHAL Headers
 #include "uhal/log/log.hpp"

// Boost Headers
#include <boost/foreach.hpp>
// #include <boost/adding.hpp>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {


    uhal::setLogLevelTo(uhal::Warning());
    // Create an amc13
    swatch::core::ParameterSet params_amc13;
    params_amc13.set("addrtabT1", "file:///opt/cactus/etc/amc13/AMC13XG_T1_v0x4002.xml");
    params_amc13.set("uriT1", "chtcp-2.0://127.0.0.1:20304?target=192.168.1.130:50001");
    params_amc13.set("addrtabT2", "file:///opt/cactus/etc/amc13/AMC13XG_T2_v0x21.xml");
    params_amc13.set("uriT2", "chtcp-2.0://127.0.0.1:20304?target=192.168.1.131:50001");
    params_amc13.set("crate", "s2g20-10");
    params_amc13.set("slot", (uint32_t) 13);
    
    swatch::hardware::AMC13Service* amc13_A = 0x0;
    try {
        amc13_A = new swatch::hardware::AMC13Service("myfirstamc13", params_amc13); 
    } catch (...) {
        std::cout << "Crap..." << std::endl;
    }

    // Mimic what System configure will need to do
    std::vector<uint32_t> slots;
    // enable slot 10
    slots.push_back(10);
    amc13_A->enableTTC(slots);

    // exit(0);
    
    // Create an MP7
    const std::string addrtab = "file:///opt/cactus/etc/mp7/addrtab/mp7fw_v1_4_0_xe/mp7xe_infra.xml";
    const std::string uri = "chtcp-2.0://127.0.0.1:20304?target=192.168.2.95:50001";

    swatch::core::ParameterSet params_mp7;
    params_mp7.set("addrtab", addrtab);
    params_mp7.set("uri", uri);
    params_mp7.set("crate", "s2g20-10");
    params_mp7.set("slot", (uint32_t) 10);

    swatch::hardware::MP7Processor* mp7_A=0x0;
    try {
        mp7_A = new swatch::hardware::MP7Processor("myfirstmp7", params_mp7);
    } catch (...) {
        std::cout << "Crap..." << std::endl;
    }
    
    BOOST_FOREACH( const std::string& mode, mp7_A->getModes() ) {
        std::cout << " - " << mode << std::endl;
    }
    
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

