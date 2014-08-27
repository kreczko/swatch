/* 
 * File:   threadExample.cxx
 * Author: ale
 *
 * Created on August 23, 2014, 6:34 PM
 */

#include <cstdlib>

// Boost Headers
#include <boost/thread.hpp>

// uhal Headers
#include "uhal/ConnectionManager.hpp"

#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

using std::cout;
using std::endl;

struct callable;


struct callable {
    int status;
    bool terminate_;
    bool dead;
private:
    pid_t pid_;
//    int handle_;
public:

    callable() : status(0), terminate_(false), dead( false ), pid_(0) {
    }
    
    virtual ~callable()  {
        // terminate())

    }
    
    pid_t pid() { return pid_; }

    void terminate() {
        // To be cleaned up
        if ( dead || pid_ == 0) return;
        cout << "Killing subprocess" << endl;
        int code = ::kill(pid_, SIGTERM);
        dead = true;
        cout << "Killing code: " << code <<  endl;
    }
    
    
    void operator()() {
        // TODOs:
        // To be turned into parameters:
        // - Logfile name
        // - Port number
        // - Verbosity
        //
        // Configurable registers
        // - use a std::map/boost::unordered_map <std::string, uint32_t>
        
        
        const char* dhwu = "/opt/cactus/bin/uhal/tests/DummyHardwareUdp.exe";
        
        pid_ = fork();
        if (pid_ == 0) {
            /* This is the child process.  Execute the shell command. */

            // Redirect stdout and stderr to logfile
            int fd = open("dummyhardwareudp.log",O_CREAT | O_WRONLY );
            dup2(fd, 1);
            dup2(fd, 2);

            execl(dhwu, dhwu,"-p50001","-v2","-V",NULL);
            _exit(EXIT_FAILURE);
        } else if (pid_ < 0) {
            /* The fork failed.  Report failure.  */
            status = -1;
            dead = true;
        } else {
            cout << "Started server with pid " << pid_ << endl;
            try {
                cout << "Wait a moment to let the Dummy to start" << endl;
                sleep(2);
                uhal::ConnectionManager c("file://etc/connections.xml");
                uhal::HwInterface b = c.getDevice("DummyProcessor");

                b.getNode("ctrl.id.magic").write(0xc0de9);
                b.getNode("ctrl.id.fwrev").write(0x11223344);
                b.getNode("ctrl.id.infos.n_rx").write(32);
                b.getNode("ctrl.id.infos.n_rx").write(18);

                b.dispatch();
                
                while(waitpid(pid_, &status, 0) != pid_) {
                    if ( terminate_ ) {
                        terminate();
                        break;
                    }
                    boost::this_thread::sleep(boost::posix_time::milliseconds(100));

                }
                cout << "Dummy Hardware is dead!" << endl;

            } catch (...) {
                // Any problem? Shoot the server
                terminate();
            }
        }
    }
};

class DummyServer {
public:
    DummyServer() {}
    virtual ~DummyServer() {
    }
    
    void start() {
        t_ = boost::thread( boost::ref(x_) );
    }
//private:
    callable x_;
    boost::thread t_;
};
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    uhal::setLogLevelTo(uhal::Warning());
    DummyServer s;
    s.start();
    
    cout << "Do we have a pid? " << s.x_.pid() << endl;
    
    std::cout << "Waiting for thread to do stuffs" << std::endl;
    for (int k(10); k > 0; --k) {
        cout << k << " pid " <<  s.x_.pid() << endl;
        sleep(1);
    }
    
    s.x_.terminate();

//    EPERM;
    return 0;
}

