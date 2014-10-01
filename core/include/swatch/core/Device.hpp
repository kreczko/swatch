/**
 * @file    Device.hpp
 * @author  Alessandro Thea
 * @brief   Device, class representing the trigger network node.
 * @date    20.08.2014
 *
 * Detailed description
 *
 */

#ifndef SWATCH_CORE_DEVICE_HPP
#define	SWATCH_CORE_DEVICE_HPP

#include "swatch/core/Object.hpp"


namespace swatch {
namespace core {

class InputPort;
class OutputPort;

/**
 *  Trigger network device
 */
class Device : public Object {
public:
    Device( const std::string& aId, const ParameterSet& params  = ParameterSet() );
    virtual ~Device();

    std::deque<InputPort*>& getInputs() { return inputs_; }
    std::deque<OutputPort*>& getOutputs() { return outputs_; }

    void halt(const ParameterSet& params = ParameterSet());
    void configure(const ParameterSet& params = ParameterSet());

    enum FsmStates {HALTED, CONFIGURED, STOPPED, ENABLED, SUSPENDED};

    FsmStates getFSM();
    void setFSMState(FsmStates state);
        

protected:

    void addInput( InputPort* aInput );
    void addOutput( OutputPort*  aOutput );

    virtual bool c_halt();
    virtual void f_halt(const ParameterSet& params = ParameterSet());
    virtual bool c_configure();
    virtual void f_configure(const ParameterSet& params = ParameterSet());

        
    std::deque<InputPort*> inputs_;
    std::deque<OutputPort*> outputs_;

    // Implementation for the dummy state machine
    FsmStates fsm_;


};
//typedef boost::shared_ptr<Device> DevicePtr;

}
}

#endif	/* SWATCH_CORE_DEVICE_HPP */

