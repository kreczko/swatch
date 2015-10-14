/* 
 * @file    DaqTTCManager.hpp
 * @author  Alessandro Thea
 * @date    July 2014
 */

#ifndef __SWATCH_DTM_DAQTTCMANAGER_HPP__
#define __SWATCH_DTM_DAQTTCMANAGER_HPP__


// C++ headers
#include <string>

// boost headers
#include "boost/noncopyable.hpp"

// SWATCH headers
#include "swatch/core/ActionableObject.hpp"
#include "swatch/core/StateMachine.hpp"
#include "swatch/dtm/DaqTTCStub.hpp"
#include "EVBInterface.hpp"


namespace swatch {
namespace dtm {

    
class DaqTTCManager;
class TTCInterface;
class AMCPortCollection;
class SLinkExpress;
class EVBInterface;


struct RunControlFSM : public boost::noncopyable {
  static const std::string kId;
  static const std::string kStateInitial;
  static const std::string kStateError;
  static const std::string kStateClockOK;
  static const std::string kStateCfg;
  static const std::string kStateRunning;
  static const std::string kStatePaused;

  static const std::string kTrColdReset;
  static const std::string kTrClockSetup;
  static const std::string kTrCfgDaq;
  static const std::string kTrStart;
  static const std::string kTrPause;
  static const std::string kTrResume;
  static const std::string kTrStop;

  core::StateMachine& fsm;
  core::StateMachine::Transition& coldReset;
  core::StateMachine::Transition& clockSetup;    
  core::StateMachine::Transition& cfgDaq;    
  core::StateMachine::Transition& start;    
  core::StateMachine::Transition& pause;    
  core::StateMachine::Transition& resume;    
  core::StateMachine::Transition& stopFromPaused;    
  core::StateMachine::Transition& stopFromRunning;    

  RunControlFSM(core::StateMachine& aFSM);
  
private:
  static core::StateMachine& addStates(core::StateMachine& aFSM);
};


class DaqTTCManager : public core::ActionableObject {
protected:
  explicit DaqTTCManager( const swatch::core::AbstractStub& aStub );

public:
  virtual ~DaqTTCManager();
    
  const DaqTTCStub& getStub() const;
    
  uint32_t getSlot() const;

  const std::string& getCrateId() const;

  uint16_t getFedId() const;

  static const std::vector<std::string> defaultMetrics;
    
  static const std::vector<std::string> defaultMonitorableObjects;

  virtual const std::vector<std::string>& getGateKeeperTables() const;

  //! Returns this amc13's TTC interface
  TTCInterface& getTTC();
    
  //! Returns this amc13's link interface
  dtm::AMCPortCollection& getAMCPorts();

protected:
      //! Register the supplied (heap-allocated) TTC interface; the child base class takes ownership of the TTC interface instance.
  TTCInterface& registerInterface( TTCInterface* aTTCInterface );

  //! Register the supplied (heap-allocated) SLink interface; the child base class takes ownership of the SLink interface instance.
  SLinkExpress& registerInterface( SLinkExpress* aSLink );

  //! Register the supplied (heap-allocated) amc ports interface; the child base class takes ownership of the link interface instance.
  AMCPortCollection& registerInterface( dtm::AMCPortCollection* aPortCollection );
  
  //! Register the supplied (heap-allocated) EvB interface; the child base class takes ownership of the EvB interface instance.
  EVBInterface& registerInterface(EVBInterface* aEventBuilder);

  RunControlFSM& getRunControlFSM();

private:
  DaqTTCStub stub_;

  mutable std::vector<std::string> gateKeeperTables_;
  
  //!
  TTCInterface* mTTC;

  //!!
  SLinkExpress* mSLink;

  //! AMC Backplane links collection
  AMCPortCollection* mAMCPorts;
  
  //! Event Builder interface
  EVBInterface* mEvb;
  
  //! Run control State Machine
  RunControlFSM mRunControlFSM;
    
protected:
    //! Metric for FED ID
  core::Metric<uint16_t>& daqMetricFedId_;
  
private:
  DaqTTCManager( const DaqTTCManager& other ); // non copyable
  DaqTTCManager& operator=( const DaqTTCManager& ); // non copyable

};

DEFINE_SWATCH_EXCEPTION(DaqTTCManagerInterfaceAlreadyDefined);

} // namespace dtm
} // namespace swatch

#endif  /* __SWATCH_DTM_DAQTTCMANAGER_HPP__ */


