/* 
 * @file    DaqTTCManager.hpp
 * @author  Alessandro Thea
 * @date    July 2014
 */

#ifndef __SWATCH_DTM_DAQTTCMANAGER_HPP__
#define __SWATCH_DTM_DAQTTCMANAGER_HPP__


// C++ headers
#include <string>

// SWATCH headers
#include "swatch/core/ActionableObject.hpp"
#include "swatch/core/StateMachine.hpp"
#include "swatch/dtm/DaqTTCStub.hpp"


namespace swatch {
namespace dtm {

    
class DaqTTCManager;


struct RunControlFSM {
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

    // virtual void reset() = 0;
    
    // /// Enables ttc commands on the given slots
    // virtual void enableTTC( const std::vector<uint32_t> & aSlots ) = 0;

    // virtual void configureClock(const std::string& mode) = 0;

    static const std::vector<std::string> defaultMetrics;

    virtual const std::vector<std::string>& getGateKeeperTables() const;

private:
    DaqTTCStub stub_;

    mutable std::vector<std::string> gateKeeperTables_;
    
protected:
    //! Metric for measured TTC clock frequency [Hz]
    core::Metric<double>& ttcMetricClockFreq_;
    //! Metric for TTC BC0 counter
    core::Metric<uint32_t>& ttcMetricBC0Counter_;
    //! Metric for TTC BC error counter
    core::Metric<uint32_t>& ttcMetricBC0Errors_;
    //! Metric for TTC single-bit error counter
    core::Metric<uint32_t>& ttcMetricSingleBitErrors_;
    //! Metric for TTC double-bit error counter
    core::Metric<uint32_t>& ttcMetricDoubleBitErrors_;
    //! Metric for FED ID
    core::Metric<uint16_t>& daqMetricFedId_;

    RunControlFSM mRunControl;
    
private:
    DaqTTCManager( const DaqTTCManager& other ); // non copyable
    DaqTTCManager& operator=( const DaqTTCManager& ); // non copyable

};


} // namespace system
} // namespace swatch



#endif	/* __SWATCH_DTM_DAQTTCMANAGER_HPP__ */
