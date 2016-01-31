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
  //! ID string of the FSM
  static const std::string kId;
  //! Initial state (i.e. halted)
  static const std::string kStateInitial;
  //! Error state
  static const std::string kStateError;
  //! 'Clock OK' state
  static const std::string kStateClockOK;
  //! Configured state
  static const std::string kStateConfigured;
  //! Running state
  static const std::string kStateRunning;
  //! Paused state
  static const std::string kStatePaused;

  //! ID string for the 'cold reset' transition (initial state to initial state)
  static const std::string kTrColdReset;
  //! ID string for the 'clock setup' transition (initial state to 'clock OK' state)
  static const std::string kTrClockSetup;
  //! ID string for the 'configure DAQ' transition ('clock OK' state to configured state)
  static const std::string kTrCfgDaq;
  //! ID string for the start transition (configred state to running state)
  static const std::string kTrStart;
  //! ID string for the pause transition (running state to paused state)
  static const std::string kTrPause;
  //! ID string for the resume transition (paused state to running state)
  static const std::string kTrResume;
  //! ID string for the stop transition (running/paused state to configured state)
  static const std::string kTrStop;

  //! The run control FSM object
  core::StateMachine& fsm;
  //! The 'cold reset' transition (initial state to initial state)
  core::StateMachine::Transition& coldReset;
  //! The 'clock setup' transition (initial state to 'clock OK' state)
  core::StateMachine::Transition& clockSetup;
  //! The 'configure DAQ' transition ('clock OK' state to configured state)
  core::StateMachine::Transition& cfgDaq;
  //! The 'start' transition (configured state to running state)
  core::StateMachine::Transition& start;
  //! The 'pause' transition (running state to paused state)
  core::StateMachine::Transition& pause;
  //! The 'resume' transition (paused state to running state)
  core::StateMachine::Transition& resume;
  //! The 'stop' transition, from paused state to configured state
  core::StateMachine::Transition& stopFromPaused;
  //! The 'stop' transition, from running state to configured state
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

  static const uint32_t kNumAMCPorts;

  static const std::vector<std::string> kDefaultMetrics;
    
  static const std::vector<std::string> kDefaultMonitorableObjects;

  virtual const std::vector<std::string>& getGateKeeperContexts() const;

  //! Returns this amc13's TTC interface
  TTCInterface& getTTC();
    
  //! Returns this amc13's link interface
  const dtm::AMCPortCollection& getAMCPorts() const;
    
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
  DaqTTCStub mStub;

  mutable std::vector<std::string> mGateKeeperTables;
  
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
  DaqTTCManager( const DaqTTCManager& ); // non copyable
  DaqTTCManager& operator=( const DaqTTCManager& ); // non copyable

};

DEFINE_SWATCH_EXCEPTION(DaqTTCManagerInterfaceAlreadyDefined);

} // namespace dtm
} // namespace swatch

#endif  /* __SWATCH_DTM_DAQTTCMANAGER_HPP__ */


