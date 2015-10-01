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
#include "swatch/core/ActionableSystem.hpp"
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

struct SysRunControlFSM {
  static const std::string kId;
  static const std::string kStateInitial;
  static const std::string kStateError;
  static const std::string kStateSync;
  static const std::string kStatePreCfg;
  static const std::string kStateCfg;
  static const std::string kStateRunning;
  static const std::string kStatePaused;

  static const std::string kTrColdReset;
  static const std::string kTrSetup;
  static const std::string kTrPreCfg;
  static const std::string kTrConnect;
  static const std::string kTrStart;
  static const std::string kTrPause;
  static const std::string kTrResume;
  static const std::string kTrStop;
  
  core::SystemStateMachine& fsm;
  core::SystemTransition& coldReset;
  core::SystemTransition& setup;
  core::SystemTransition& preconfigure;
  core::SystemTransition& connect;
  core::SystemTransition& start;
  core::SystemTransition& pause;
  core::SystemTransition& resume;
  core::SystemTransition& stopFromPaused;
  core::SystemTransition& stopFromRunning;
  
  SysRunControlFSM(core::SystemStateMachine& aFSM);
  
private:
  static core::SystemStateMachine& addStates(core::SystemStateMachine& aFSM);
};

//! Generic class representing a system of one ore mores processors
class System : public core::ActionableSystem {
public:
  
  typedef boost::unordered_map<std::string, Crate*> CratesMap;

  System( const swatch::core::AbstractStub& aStub );
  virtual ~System();
    
  const SystemStub& getStub() const;
    
  const std::deque<processor::Processor*>& getProcessors() ;
  const std::deque<Service*>& getServices();
  const std::deque<DaqTTCManager*>& getDaqTTC();
  const std::deque<processor::Link*>& getLinks();
  const CratesMap& getCrates();
    
  bool hasCrate(const std::string& aCrateId) const;

protected:
  virtual void retrieveMetricValues() {}

  
  SysRunControlFSM mRunControl;
  //! List of external ports
  // std::deque<SysPorts*> mPorts;    

private:
    
    void add( processor::Processor* aProcessor );
    void add( system::DaqTTCManager* aAMC13 );
    void add( processor::Link* aLink );
    void add( system::Service* aService );
    void add( system::Crate* aCrate );
  
    void addCrates();
    void addProcessors();
    void addDaqTTCs();
    void addLinks();

    const SystemStub stub_;

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

DEFINE_SWATCH_EXCEPTION(SystemConstructionFailed);

}
}

#endif	/* __SWATCH_SYSTEM_SYSTEM_HPP__ */

