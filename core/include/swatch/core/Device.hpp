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
class Device {
public:
    Device();
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

    typedef std::deque<InputPort*> tInputPortDeque;
    typedef std::deque<OutputPort*> tOutputPortDeque;

//protected:

    void addInput( InputPort* aInput );
    void addOutput( OutputPort*  aOutput );

    tInputPortDeque inputs_;
    tOutputPortDeque outputs_;

};

}
}

#endif	/* __SWATCH_CORE_DEVICE_HPP__ */

