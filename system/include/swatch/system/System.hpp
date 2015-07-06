/* 
 * File:   System.hpp
 * Author: ale
 *
 * Created on July 13, 2014, 11:20 AM
 */

#ifndef __SWATCH_SYSTEM_SYSTEM_HPP__
#define	__SWATCH_SYSTEM_SYSTEM_HPP__

// C++ Headers
#include <string>

// boost headers
#include <boost/unordered_map.hpp>

// Swatch Headers
#include "swatch/core/ActionableObject.hpp"
#include "swatch/system/SystemStub.hpp"


// Forward declarations
namespace swatch {

namespace processor {
	class Processor;
  class Link;
}

namespace system {

class Crate;
class Service;
class DaqTTCManager;

//! Generic class to build a 
class System : public core::ActionableObject {
public:
  
  typedef boost::unordered_map<std::string, Crate*> CratesMap;

    System( const swatch::core::AbstractStub& aStub );
    virtual ~System();
    
    const SystemStub& getStub() const;
    
    void add( processor::Processor* aProcessor );
    void add( system::DaqTTCManager* aAMC13 );
    void add( processor::Link* aLink );
    void add( system::Service* aService );
    void add( system::Crate* crate );
    
    std::deque<processor::Processor*>& getProcessors() ;
    std::deque<Service*>& getServices();
    std::deque<DaqTTCManager*>& getDaqTTC();
    std::deque<processor::Link*>& getLinks();
    CratesMap& getCrates();
    
    bool hasCrate(const std::string& crate_id) const;


protected:
  
    virtual void implementUpdateMetrics() {}

    //! List of external ports
    // std::deque<SysPorts*> mPorts;
    

private:
    SystemStub stub_;

    //! List of processors
    std::deque<processor::Processor*> processors_;
    
    //! List of AMC13s
    std::deque<DaqTTCManager*> daqTtc_;
    
    //! List of services
    std::deque<Service*> services_;
    
    //! List of internal links
    std::deque<processor::Link*> links_; 

    //! Map of crates
    CratesMap cratesMap_;
};

}
}

#endif	/* __SWATCH_SYSTEM_SYSTEM_HPP__ */

