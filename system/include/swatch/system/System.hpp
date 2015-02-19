/* 
 * File:   System.hpp
 * Author: ale
 *
 * Created on July 13, 2014, 11:20 AM
 */

#ifndef __SWATCH_TEST_SYSTEM_HPP__
#define	__SWATCH_TEST_SYSTEM_HPP__


// Swatch Headers
#include "swatch/core/Device.hpp"


// Forward declarations
namespace swatch {

namespace processor
{
	class Processor;
}

namespace core
{
	class Link;
}

}

namespace swatch {
namespace system {

class Crate;
class Service;
class DaqTTCService;

//! Generic class to build a 
class System : public core::Device {
public:

    System( const std::string& aId, const core::XParameterSet& params = core::XParameterSet() );
    virtual ~System();
    
    void add( processor::Processor* aProcessor );
    void add( system::DaqTTCService* aAMC13 );
    void add( core::Link* aLink );
    void add( system::Service* aService );
    void add( system::Crate* crate );
    
    const std::deque<processor::Processor*>& getProcessors() const;
    const std::deque<Service*>& getServices() const;
    const std::deque<core::Link*>& getLinks() const;
    const boost::unordered_map<std::string, Crate*>& getCrates() const;
    
    bool hasCrate(const std::string& crate_id) const;


    
protected:

    //! List of processors
    std::deque<processor::Processor*> processors_;
    
    //! List of AMC13s
    std::deque<DaqTTCService*> amc13s_;
    
    //! List of services
    std::deque<Service*> services_;
    
    //! List of internal links
    std::deque<core::Link*> links_; 
    
    //! List of external ports
    // std::deque<SysPorts*> mPorts;
    
    //! Map of crates
    boost::unordered_map<std::string, Crate*> cratesMap_;



};

}
}

#endif	/* __SWATCH_TEST_SYSTEM_HPP__ */

