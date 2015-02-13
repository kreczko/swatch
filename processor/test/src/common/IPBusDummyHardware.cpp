/**
 * @file    IPBusDummyHardware.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date 
 */

#include "swatch/processor/test/IPBusDummyHardware.hpp"

// Swatch Headers
#include "swatch/core/Utilities.hpp"
#include "swatch/core/XParameterSet.hpp"

// C++ Headers
#include <sys/prctl.h>
#include <xdata/UnsignedInteger.h>

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
    while (counts--) {
        if (started()) return;
        core::millisleep(100);
    }

    if (!counts) {
        throw std::runtime_error("Timed out when waiting for dummyhardware to start");
    }
}

void
IPBusDummyHardware::terminate() {

    if (workers_) delete workers_;
    workers_ = 0x0;
    // To be cleaned up
    if (pid_ != 0) {
        cout << "Killing subprocess" << endl;
        int code = ::kill(pid_, SIGTERM);
        cout << "Killing code: " << code << endl;
        // Fix logfile permissions
        std::string logfile = "udp-" + name_ + ".log";
        chmod(logfile.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        pid_ = 0;
    }

    if (hw_) delete hw_;
}

void
IPBusDummyHardware::run() {
    // TODOs:
    // To be turned into parameters:
    // - Verbosity

    const char* udpexe = "/opt/cactus/bin/uhal/tests/DummyHardwareUdp.exe";

    pid_ = fork();
    if (pid_ == 0) {
        /* This is the child process.  Execute the shell command. */
        // But first 
        ::prctl(PR_SET_PDEATHSIG, SIGHUP);

        std::string logfile = "udp-" + name_ + ".log";
        // Redirect stdout and stderr to logfile
        int fd = open(logfile.c_str(), O_CREAT | O_WRONLY);
        dup2(fd, 1);
        dup2(fd, 2);

        std::stringstream ssPort;
        ssPort << "-p" << port_;
        execl(udpexe, udpexe, ssPort.str().c_str(), "-v2", "-V", NULL);
        _exit(EXIT_FAILURE);

    } else if (pid_ < 0) {
        /* The fork failed.  Report failure.  */
        status_ = -1;
    } else {
        // It forked. Incredible.
        cout << "Started server with pid " << pid_ << endl;
        try {
            cout << "Wait a sec for " << name_ << " to start" << endl;
            sleep(1);

            uhal::setLogLevelTo(uhal::WarningLevel());
            std::stringstream ssURI;
            ssURI << "ipbusudp-2.0://127.0.0.1:" << port_;
            hw_ = new uhal::HwInterface(uhal::ConnectionManager::getDevice(
                    name_,
                    ssURI.str().c_str(),
                    addrtab_
                    )
                    );

            started_ = true;

            // waits until the process finishes
            ::waitpid(pid_, &status_, 0);

            cout << name_ << " is dead!" << endl;

        } catch (...) {
            // Any problem? Shoot the server
            terminate();
        }
    }
}

uhal::HwInterface&
IPBusDummyHardware::hw() const {
    if (!hw_)
        throw std::runtime_error("No uhal hardware interface instantiated");

    return *hw_;
}

void
IPBusDummyHardware::load(const swatch::core::XParameterSet& map) {

    BOOST_FOREACH(const std::string& p, map.keys()) {
        hw().getNode(p).write(map.get<xdata::UnsignedInteger>(p));
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
