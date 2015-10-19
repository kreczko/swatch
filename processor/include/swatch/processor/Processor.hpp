/**
 * @file    Processor.hpp
 * @author  Alessandro Thea
 * @brief   Processor abstract interface
 * @date    1 July 2014
 */

#ifndef __SWATCH_PROCESSOR_PROCESSOR_HPP__
#define __SWATCH_PROCESSOR_PROCESSOR_HPP__


// boost headers
#include "boost/noncopyable.hpp"

// SWATCH headers
#include "swatch/core/ActionableObject.hpp"
#include "swatch/core/exception.hpp"
#include "swatch/core/StateMachine.hpp"
#include "swatch/processor/ProcessorStub.hpp"


namespace swatch {

namespace core {
class AbstractMetric;
class StateMachine;
class XParameterSet;
}

namespace processor {

// Forward declarations
class TTCInterface;
class ReadoutInterface;
class AlgoInterface;
class PortCollection;
class Processor;


struct RunControlFSM : public boost::noncopyable {
  //! FSM ID string
  static const std::string kId;
  //! Initial state (i.e. halted)
  static const std::string kStateInitial;
  //! Error state
  static const std::string kStateError;
  //! Synchronised state
  static const std::string kStateSync;
  //! Configured state
  static const std::string kStateConfigured;
  //! Aligned state
  static const std::string kStateAligned;

  //! Cold reset transition (initial state to initial state)
  static const std::string kTrColdReset;
  //! ID string for the 'setup' transition (initial state to synchronised state)
  static const std::string kTrSetup;
  //! ID string for the 'configure' transition (synchronised state to configured state)
  static const std::string kTrConfigure;
  //! ID string for teh 'align' transition (configured state to aligned state)
  static const std::string kTrAlign;

  //! The run control FSM object
  core::StateMachine& fsm;
  //! The 'cold reset' transition (initial state to initial state)
  core::StateMachine::Transition& coldReset;
  //! The 'setup' transition (initial state to synchronised state)
  core::StateMachine::Transition& setup;
  //! The 'configure' transition (synchronised state to configured state)
  core::StateMachine::Transition& configure;
  //! The 'align' transition (configured state to aligned state)
  core::StateMachine::Transition& align;

  RunControlFSM(core::StateMachine& aFSM);

private:
  static core::StateMachine& addStates(core::StateMachine& aFSM);
};



class Processor : public core::ActionableObject {
protected:
  Processor(const swatch::core::AbstractStub& );
public:
    
    virtual ~Processor();
 
    const ProcessorStub& getStub() const;

    /**
     * Returns the board slot number
     * @return the board's slot number in the crate
     */
    uint32_t getSlot() const;
    
    /**
     * Processor crate ID getter
     * @return ID of the crate this processor that this processor is in
     */
    const std::string& getCrateId() const;

    //! Constant corresponding to no slot being assigned
    static const uint32_t NoSlot;
    
//    /**
//     * Additional firmware information.
//     * The string is meant to be informative for the user
//     * @details [long description]
//     * @return String containing additional firmware informations
//     */
//    virtual std::string firmwareInfo() const = 0; 

    //! Returns this processor's TTC interface
    TTCInterface& getTTC();
    
    //! Returns this processor's readout interface
    ReadoutInterface& getReadout();
    
    //! Returns this processor's algo interface
    AlgoInterface& getAlgo();
    
    //! Returns this processor's link interface
    PortCollection& getPorts();
    
    static const std::vector<std::string> defaultMetrics;

    static const std::vector<std::string> defaultMonitorableObjects;

    virtual const std::vector<std::string>& getGateKeeperTables() const;


protected:

    //! Register the supplied (heap-allocated) TTC interface in this processor; the processor base class takes ownership of the TTC interface instance.
    TTCInterface& registerInterface( TTCInterface* aTTCInterface );

    //! Register the supplied (heap-allocated) readout interface in this processor; the processor base class takes ownership of the readout interface instance.
    ReadoutInterface& registerInterface( ReadoutInterface* aReadoutInterface );

    //! Register the supplied (heap-allocated) algo interface in this processor; the processor base class takes ownership of the algo interface instance.
    AlgoInterface& registerInterface( AlgoInterface* aAlgoInterface );

    //! Register the supplied (heap-allocated) link interface in this processor; the processor base class takes ownership of the link interface instance.
    PortCollection& registerInterface( PortCollection* aPortCollection );

    //! Firmware version metric
    core::Metric<uint64_t>& metricFirmwareVersion_;

    RunControlFSM& getRunControlFSM();
    
private:
    const ProcessorStub stub_;

    //! TTC control interface
    TTCInterface* mTTC;

    //! Readout control interface
    ReadoutInterface* mReadout;

    //! Algorithm control interface
    AlgoInterface* mAlgo;

    //! Optical link interface
    PortCollection* mPorts;

    mutable std::vector<std::string> gateKeeperTables_;
    
    RunControlFSM mRunControlFSM;

private:
    Processor( const Processor& other ); // non copyable
    Processor& operator=( const Processor& ); // non copyable
};


DEFINE_SWATCH_EXCEPTION(ProcessorInterfaceAlreadyDefined);

}
}

#endif	/* __SWATCH_PROCESSOR_PROCESSOR_HPP__ */

