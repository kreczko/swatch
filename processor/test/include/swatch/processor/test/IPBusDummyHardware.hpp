/**
 * @file    IpbusDummyHardware.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    03/09/2014
 */

#ifndef __SWATCH_PROCESSOR_TEST_IPBUSDUMMYHARDWARE_HPP__
#define __SWATCH_PROCESSOR_TEST_IPBUSDUMMYHARDWARE_HPP__

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

// Swatch Headers
#include "swatch/core/XParameterSet.hpp"

namespace swatch {
namespace processor {
namespace test {

struct IPBusWorkLoop {
    IPBusWorkLoop( uint32_t millisec ) : millisec_(millisec), terminate_(false) {}
    virtual void operator()( uhal::HwInterface* hw ) = 0;

    void terminate() { terminate_ = true; }
private:
    friend class IPBusDummyHardware;

    void run( uhal::HwInterface* hw ) {
        while( true ) {
          if ( terminate_ ) break;

          (*this)(hw);
          boost::this_thread::sleep_for(boost::chrono::milliseconds(millisec_));
        }
        
        std::cout << "Workloop terminated" << std::endl;
    }

    //!
    uint32_t millisec_;
    bool terminate_;
};

class IPBusDummyHardware : private boost::noncopyable {
public:
    
    IPBusDummyHardware(const std::string& name, uint32_t port, const std::string& addrtab);
    virtual ~IPBusDummyHardware();

    pid_t pid() const;
    void start();
    void terminate();
    void run();

    uhal::HwInterface& hw() const;
    
    void load( const swatch::core::XParameterSet& map );

    bool started() const { return started_; }
    
    void add( IPBusWorkLoop* w );
    
private:

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
    uhal::HwInterface* hw_;

    std::vector<IPBusWorkLoop*> workers_;
    
    boost::thread_group* threads_;

};

}
}
}

#endif  /* __SWATCH_PROCESSOR_TEST_IPBUSDUMMYHARDWARE_HPP__ */

