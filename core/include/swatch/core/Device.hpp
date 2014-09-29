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
        

protected:

    void addInput( InputPort* aInput );
    void addOutput( OutputPort*  aOutput );

        
    std::deque<InputPort*> inputs_;
    std::deque<OutputPort*> outputs_;


};
//typedef boost::shared_ptr<Device> DevicePtr;

}
}

#endif	/* SWATCH_CORE_DEVICE_HPP */

