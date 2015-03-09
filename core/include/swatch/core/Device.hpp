/**
 * @file    Device.hpp
 * @author  Alessandro Thea
 * @brief   Device, class representing the trigger network node.
 * @date    20.08.2014
 *
 * Detailed description
 *
 */

#ifndef __SWATCH_CORE_DEVICE_HPP__
#define	__SWATCH_CORE_DEVICE_HPP__

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
    Device( const std::string& aId, const XParameterSet& params  = XParameterSet() );
    virtual ~Device();


    /**
     * Number of input channels
     * @return Number of input channels
     */
    uint32_t getNumInputs() const { return inputs_.size(); }

    /**
     * Number of output channels
     * @return Number of output channels
     */
    uint32_t getNumOutputs() const { return outputs_.size(); }

    std::deque<InputPort*>& getInputs() { return inputs_; }

    std::deque<OutputPort*>& getOutputs() { return outputs_; }

protected:

    void addInput( InputPort* aInput );
    void addOutput( OutputPort*  aOutput );

    std::deque<InputPort*> inputs_;
    
    std::deque<OutputPort*> outputs_;

};

}
}

#endif	/* __SWATCH_CORE_DEVICE_HPP__ */

