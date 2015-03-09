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
#include "swatch/core/ActionHandler.hpp"


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
class System : public core::Device, public core::ActionHandler {
public:
  
  typedef boost::container::map<std::string, Crate*> CratesMap;

    System( const std::string& aId, const core::XParameterSet& params = core::XParameterSet() );
    virtual ~System();
    
    void add( processor::Processor* aProcessor );
    void add( system::DaqTTCService* aAMC13 );
    void add( core::Link* aLink );
    void add( system::Service* aService );
    void add( system::Crate* crate );
    
    std::deque<processor::Processor*>& getProcessors() ;
    std::deque<Service*>& getServices();
    std::deque<DaqTTCService*>& getDaqTTC();
    std::deque<core::Link*>& getLinks();
    CratesMap& getCrates();
    
    bool hasCrate(const std::string& crate_id) const;


    
protected:

    //! List of processors
    std::deque<processor::Processor*> processors_;
    
    //! List of AMC13s
    std::deque<DaqTTCService*> daqTtc_;
    
    //! List of services
    std::deque<Service*> services_;
    
    //! List of internal links
    std::deque<core::Link*> links_; 
    
    //! List of external ports
    // std::deque<SysPorts*> mPorts;
    
    //! Map of crates
    CratesMap cratesMap_;



};

}
}

#endif	/* __SWATCH_TEST_SYSTEM_HPP__ */

