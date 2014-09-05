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
    
    typedef boost::unordered_map<std::string,uint32_t> RegisterMap;

    IpbusDummyHardware(const std::string& name, uint32_t port, const std::string& addrtab);
    virtual ~IpbusDummyHardware();

    pid_t pid() const;
    void start();
    void terminate();
    void run();

    uhal::HwInterface& hw() const;
    
    void load( const RegisterMap& map );

    bool started() const { return started_; }

private:
    // uhal::HwInterface getHwInterface( const std::string& addrtab );

    std::string name_;

    uint32_t port_;

    //! process id
    pid_t pid_;

    //! udp hw return status (do we need it?)
    int status_;

    //!
    bool started_;

    //!
    std::string addrtab_;

    //!
    boost::thread thread_;

    //!
    uhal::HwInterface* hw_;
};

}
}
}

#endif  /* SWATCH_PROCESSOR_TEST_IPBUSDUMMYHARDWARE_HPP */

