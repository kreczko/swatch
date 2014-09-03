/* 
 * File:   threadExample.cxx
 * Author: ale
 *
 * Created on August 23, 2014, 6:34 PM
 */

#include <cstdlib>

// Boost Headers
#include <boost/thread.hpp>
#include <boost/foreach.hpp>

// uhal Headers
#include "uhal/ConnectionManager.hpp"

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

using std::cout;
using std::endl;

class DummyHardware {
private:

    typedef boost::unordered_map<std::string, uint32_t> RegisterMap;

    std::string name_;
    uint32_t port_;
    RegisterMap registers_;

    //! process id
    pid_t pid_;

    //! udp hw return status (do we need it?)
    int status_;

    //! 
    bool dead_;
    boost::thread thread_;


public:

    DummyHardware(const std::string& name, uint32_t port) : name_(name), port_(port), pid_(0), status_(0), dead_(false) {
        
        // This shall become a parameter
        registers_["ctrl.id.magic"] = 0xc0de9;
        registers_["ctrl.id.fwrev"] = 0x11223344;
        registers_["ctrl.id.infos.n_rx"] = 32;
        registers_["ctrl.id.infos.n_rx"] = 18;
    }

    virtual ~DummyHardware() {
        terminate();
    }

    void start() {
        // start the hardware
        thread_ = boost::thread(&DummyHardware::run, this);
    }

    pid_t pid() {
        return pid_;
    }

    void terminate() {
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

    void run() {
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
                        "file://${SWATCH_ROOT}/processor/tests/etc/dummy.xml");

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
};

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    uhal::setLogLevelTo(uhal::Warning());

    DummyHardware x("ararar", 50010);
    x.start();
    DummyHardware y("zzzzzz", 50011);
    y.start();

    cout << "Do we have a pid? " << x.pid() << endl;

    std::cout << "Waiting for thread to do stuffs" << std::endl;
    for (int k(10); k > 0; --k) {
        cout << k << " pid " << x.pid() << "|" << y.pid() << endl;
        sleep(1);
    }

    x.terminate();
    return 0;
}

