/**
 * @file    LinkInterface.hpp
 * @author  Alessandro Thea
 * @brief   Device, class representing the trigger network node.
 * @date    20.08.2014
 *
 * Detailed description
 *
 */

#ifndef __SWATCH_PROCESSOR_LINKINTERFACE_HPP__
#define __SWATCH_PROCESSOR_LINKINTERFACE_HPP__


// SWATCH headers
#include "swatch/core/MonitorableObject.hpp"


namespace swatch {
namespace processor {

class InputPort;
class OutputPort;

/**
 *  Trigger network device
 */
class LinkInterface: public core::MonitorableObject {
public:
    LinkInterface();
    virtual ~LinkInterface();


    /**
     * Number of input channels
     * @return Number of input channels
     */
    uint32_t getNumInputs() const;

    /**
     * Number of output channels
     * @return Number of output channels
     */
    uint32_t getNumOutputs() const;

    std::deque<InputPort*>& getInputs();

    std::deque<OutputPort*>& getOutputs();

    InputPort* getInput( const std::string& aId );
    OutputPort* getOutput( const std::string& aId );

    typedef std::deque<InputPort*> tInputPortDeque;
    typedef std::deque<OutputPort*> tOutputPortDeque;

//protected:

    void addInput( InputPort* aInput );
    void addOutput( OutputPort*  aOutput );
    
    
protected:
    void implementUpdateMetrics() {}

private:
    tInputPortDeque inputs_;
    tOutputPortDeque outputs_;
};

}
}

#endif	/* __SWATCH_CORE_DEVICE_HPP__ */

