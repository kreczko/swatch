/**
 * @file    IpbusDummyHardware.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    03/09/2014
 */

#ifndef SWATCH_PROCESSOR_TEST_IPBUSDUMMYHARDWARE_HPP
#define SWATCH_PROCESSOR_TEST_IPBUSDUMMYHARDWARE_HPP

// uhal Headers
#include "uhal/ConnectionManager.hpp"

// Boost Headers
#include <boost/thread.hpp>
#include <boost/foreach.hpp>

// C Headers
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace swatch {
namespace processor {
namespace test {

class IpbusDummyHardware {
public:
    IpbusDummyHardware(const std::string& name, uint32_t port);
    virtual ~IpbusDummyHardware();

    pid_t pid();
    void start();
    void terminate();
    void run();

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
};

}
}
}

#endif  /* SWATCH_PROCESSOR_TEST_IPBUSDUMMYHARDWARE_HPP */

