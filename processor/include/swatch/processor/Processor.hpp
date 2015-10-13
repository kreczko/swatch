/**
 * @file    Processor.hpp
 * @author  Alessandro Thea
 * @brief   Processor abstract interface
 * @date    1 July 2014
 */

#ifndef __SWATCH_PROCESSOR_PROCESSOR_HPP__
#define __SWATCH_PROCESSOR_PROCESSOR_HPP__


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


struct RunControlFSM {
  static const std::string kId;
  static const std::string kStateInitial;
  static const std::string kStateError;
  static const std::string kStateSync;
  static const std::string kStatePreCfg;
  static const std::string kStateCfg;

  static const std::string kTrColdReset;
  static const std::string kTrSetup;
  static const std::string kTrPreCfg;
  static const std::string kTrConnect;

  core::StateMachine& fsm;
  core::StateMachine::Transition& coldReset;
  core::StateMachine::Transition& setup;
  core::StateMachine::Transition& preconfigure;
  core::StateMachine::Transition& connect;

  RunControlFSM(core::StateMachine& aFSM);

private:
  static core::StateMachine& addStates(core::StateMachine& aFSM);

  RunControlFSM( const RunControlFSM& other ); // non copyable
  RunControlFSM& operator=( const RunControlFSM& ); // non copyable
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

