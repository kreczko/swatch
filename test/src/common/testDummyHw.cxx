/* 
 * File:   testDummyHw.cxx
 * Author: ale
 *
 * Created on July 29, 2014, 3:45 PM
 */

#include <cstdlib>
#include "unistd.h"
#include <sys/types.h>
#include <sys/wait.h>

#include "uhal/HwInterface.hpp"
#include "uhal/ConnectionManager.hpp"
#include <boost/filesystem.hpp>


using namespace std;
#define SHELL "/bin/sh"

/*
 * 
 */
int main(int argc, char** argv) {

    int status;


    const char* command = "/opt/cactus/bin/uhal/tests/DummyHardwareUdp.exe -p50001 -v2 -V";


    pid_t pid = fork();
    if (pid == 0) {
        /* This is the child process.  Execute the shell command. */
        execl(SHELL, SHELL, "-c", command, NULL);
        _exit(EXIT_FAILURE);
    } else if (pid < 0)
        /* The fork failed.  Report failure.  */
        status = -1;
    else {
        
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
        /* This is the parent process.  Wait for the child to complete.  */
        if (waitpid(pid, &status, 0) != pid)
            status = -1;
        return status;
        } catch (...) {
            cout << "Killing the Dummy Hardware" << endl;
            kill(pid, SIGKILL);
        }
    }
    return 0;

}

