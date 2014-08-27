/* 
 * File:   System.hpp
 * Author: ale
 *
 * Created on July 13, 2014, 11:20 AM
 */

#ifndef _swatch_test_system_hpp_
#define	_swatch_test_system_hpp_


// Swatch Headers
#include "swatch/processor/Processor.hpp"
#include "swatch/core/Link.hpp"
#include "swatch/system/Service.hpp"
#include "swatch/system/AMC13Service.hpp"
#include "swatch/system/Crate.hpp"

namespace swatch {
namespace system {


//! Generic class to build a 
class System : public core::Device {
public:

    System( const std::string& aId, const core::Arguments& aArgument = core::Arguments() );
    virtual ~System();
    
    void add( processor::Processor* aProcessor );
    void add( system::AMC13Service* aAMC13 );
    void add( core::Link* aLink );
    void add( system::Service* aService );
    
    const std::deque<processor::Processor*>& getProcessors() const;
    const std::deque<core::Link*>& getLinks() const;
    const boost::unordered_map<std::string, Crate*>& getCrates() const;
    
    
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
    
};

}
}

#endif	/* _swatch_test_system_hpp_ */

