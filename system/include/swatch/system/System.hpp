/* 
 * File:   System.hpp
 * Author: ale
 * Date:   July 2014
 * Created on July 13, 2014, 11:20 AM
 */

#ifndef __SWATCH_SYSTEM_SYSTEM_HPP__
#define	__SWATCH_SYSTEM_SYSTEM_HPP__


// C++ headers
#include <string>

// boost headers
#include <boost/unordered_map.hpp>

// SWATCH headers
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

//! Generic class representing a system of one ore mores processors
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
  void add( system::Crate* aCrate );

  const std::deque<processor::Processor*>& getProcessors() ;
  const std::deque<Service*>& getServices();
  const std::deque<DaqTTCManager*>& getDaqTTC();
  const std::deque<processor::Link*>& getLinks();
  const CratesMap& getCrates();
    
  bool hasCrate(const std::string& aCrateId) const;

  const std::vector<std::string>& getGateKeeperTables() const;

  
protected:
  virtual void retrieveMetricValues() {}

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
    
    std::vector<std::string> gateKeeperTables_;
};

}
}

#endif	/* __SWATCH_SYSTEM_SYSTEM_HPP__ */

