/**
 * @file    Template.cpp
 * @author  Author1, Author2
 * @brief   Brief description
 * @date 
 */

#include "swatch/processor/test/IpbusDummyHardware.hpp"

// Namespace resolution
using std::cout;
using std::endl;

namespace swatch {
namespace processor {
namespace test {

IpbusDummyHardware::IpbusDummyHardware(const std::string& name, uint32_t port) : name_(name), port_(port), pid_(0), status_(0), dead_(false) {
    // This shall become a parameter
    registers_["ctrl.id.magic"] = 0xc0de9;
    registers_["ctrl.id.fwrev"] = 0x11223344;
    registers_["ctrl.id.infos.n_rx"] = 32;
    registers_["ctrl.id.infos.n_rx"] = 18;
}

pid_t
IpbusDummyHardware::pid() {
    return pid_;
}

IpbusDummyHardware::~IpbusDummyHardware() {
}

void
IpbusDummyHardware::start() {
    // start the hardware
    thread_ = boost::thread(&IpbusDummyHardware::run, this);
}

void
IpbusDummyHardware::terminate() {
    // To be cleaned up
    if (dead_ || pid_ == 0) return;
    cout << "Killing subprocess" << endl;
    int code = ::kill(pid_, SIGTERM);
    dead_ = true;
    cout << "Killing code: " << code << endl;
    // Fix logfile permissions
    std::string logfile = "udp-" + name_ + ".log";
    chmod(logfile.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
}

void
IpbusDummyHardware::run() {
    // TODOs:
    // To be turned into parameters:
    // - Verbosity

    const char* udpexe = "/opt/cactus/bin/uhal/tests/DummyHardwareUdp.exe";

    pid_ = fork();
    if (pid_ == 0) {
        /* This is the child process.  Execute the shell command. */

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
        dead_ = true;
    } else {
        // It forked. Incredible.
        cout << "Started server with pid " << pid_ << endl;
        try {
            cout << "Wait a moment to let the Dummy to start" << endl;
            sleep(2);

            std::stringstream ssURI;
            ssURI << "ipbusudp-2.0://127.0.0.1:" << port_;
            uhal::HwInterface b = uhal::ConnectionManager::getDevice(
                    "DummyProcessor",
                    ssURI.str().c_str(),
                    "file://${SWATCH_ROOT}/processor/test/etc/dummy.xml");

            BOOST_FOREACH(RegisterMap::value_type p, registers_) {
                b.getNode(p.first).write(p.second);
            }

            b.dispatch();

            // waits until the process finishes
            ::waitpid(pid_, &status_, 0);

            cout << "Dummy Hardware " << name_ << " is dead!" << endl;

        } catch (...) {
            // Any problem? Shoot the server
            terminate();
        }
    }
}



}
}
}
