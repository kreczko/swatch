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

struct IPBusWorkLoop {
    IPBusWorkLoop( uint32_t millisec ) : millisec_(millisec) {}
    virtual void operator()( uhal::HwInterface* hw ) = 0;

private:
    friend class IPBusDummyHardware;

    void run( uhal::HwInterface* hw ) {
        while( true ) {
            (*this)(hw);
            boost::this_thread::sleep_for(boost::chrono::milliseconds(millisec_));
        }
    }

    //!
    uint32_t millisec_;
};

class IPBusDummyHardware : private boost::noncopyable {
public:
    
    typedef boost::unordered_map<std::string,uint32_t> RegisterMap;

    IPBusDummyHardware(const std::string& name, uint32_t port, const std::string& addrtab);
    virtual ~IPBusDummyHardware();

    pid_t pid() const;
    void start();
    void terminate();
    void run();

    uhal::HwInterface& hw() const;
    
    void load( const RegisterMap& map );

    bool started() const { return started_; }
    
    void add( IPBusWorkLoop* w );
    
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


    boost::thread_group* workers_;

};

}
}
}

#endif  /* SWATCH_PROCESSOR_TEST_IPBUSDUMMYHARDWARE_HPP */

