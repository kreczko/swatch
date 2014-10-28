/* 
 * File:   System.hpp
 * Author: ale
 *
 * Created on July 13, 2014, 11:20 AM
 */

#ifndef _swatch_test_system_hpp_
#define	_swatch_test_system_hpp_


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
class AMC13Service;

//! Generic class to build a 
class System : public core::Device {
public:

    System( const std::string& aId, const core::ParameterSet& params = core::ParameterSet() );
    virtual ~System();
    
    void add( processor::Processor* aProcessor );
    void add( system::AMC13Service* aAMC13 );
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
    std::deque<AMC13Service*> amc13s_;
    
    //! List of services
    std::deque<Service*> services_;
    
    //! List of internal links
    std::deque<core::Link*> links_; 
    
    //! List of external ports
    // std::deque<SysPorts*> mPorts;
    
    //! Map of crates
    boost::unordered_map<std::string, Crate*> cratesMap_;
    

    // Operations checks: might be overriden by descendants
   virtual bool canHalt();
   virtual bool canConfigure();

   // Operations: something in common in all inherited classes? (e.g., actions with the ParameterSet)
   virtual void doHalt(const core::ParameterSet& params);
   virtual void doConfigure(const core::ParameterSet& params);


};

}
}

#endif	/* _swatch_test_system_hpp_ */

