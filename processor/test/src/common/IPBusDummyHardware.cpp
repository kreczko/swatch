/**
 * @file    IPBusDummyHardware.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date 
 */

#include "swatch/processor/test/IPBusDummyHardware.hpp"

// Swatch Headers
#include "swatch/core/Utilities.hpp"

// uHAL Headers
#include "uhal/tests/UDPDummyHardware.hpp"

// C++ Headers
#include <sys/prctl.h>

// Namespace resolution
using std::cout;
using std::endl;

namespace swatch {
namespace processor {
namespace test {

IPBusDummyHardware::IPBusDummyHardware(const std::string& name, uint32_t port, const std::string& addrtab) :
    name_(name), port_(port), pid_(0), status_(0), started_(false), addrtab_(addrtab), hw_(0x0) {
        workers_ = new boost::thread_group();
}


pid_t
IPBusDummyHardware::pid() const {
    return pid_;
}


IPBusDummyHardware::~IPBusDummyHardware() {
    terminate();
}


void
IPBusDummyHardware::start() {
    // start the hardware
    thread_ = boost::thread(&IPBusDummyHardware::run, this);
    
    uint32_t counts(100);
    while( counts-- ) {
        if ( started() ) return;
        core::millisleep(100);
    }
    
    if ( !counts ) {
        throw std::runtime_error("Timed out when waiting for dummyhardware to start");
    }
}


void
IPBusDummyHardware::terminate() {

    if ( workers_) delete workers_; workers_ = 0x0;
    // To be cleaned up
    if ( pid_ != 0 ) {
        cout << "Killing subprocess" << endl;
        int code = ::kill(pid_, SIGTERM);
        cout << "Killing code: " << code << endl;
        // Fix logfile permissions
        std::string logfile = "udp-" + name_ + ".log";
        chmod(logfile.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
        pid_ = 0;
    }

    if ( hw_ ) delete hw_; 
}

void IPBusDummyHardware::run() {

    // Create the hw interface
    uhal::setLogLevelTo(uhal::WarningLevel());
    std::stringstream ssURI;
    ssURI << "ipbusudp-2.0://127.0.0.1:" << port_;
    hw_ = new uhal::HwInterface( uhal::ConnectionManager::getDevice(
            name_,
            ssURI.str().c_str(),
            addrtab_
            )
    );

    started_ = true;

    // And then start the interface
    uhal::tests::UDPDummyHardware<2,0> hardware(port_, 0, false);
    hardware.run();
}


uhal::HwInterface&
IPBusDummyHardware::hw() const {
    if ( !hw_ )
        throw std::runtime_error("No uhal hardware interface instantiated");
    
    return *hw_;
}


void
IPBusDummyHardware::load(const IPBusDummyHardware::RegisterMap& map) {
    BOOST_FOREACH(RegisterMap::value_type p, map) {
        hw().getNode(p.first).write(p.second);
    }

    hw().dispatch();   
}

void IPBusDummyHardware::add(IPBusWorkLoop* w) {
   workers_->add_thread( 
    new boost::thread(&IPBusWorkLoop::run, w, hw_)
   );
}

}
}
}
