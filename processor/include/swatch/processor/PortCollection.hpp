/**
 * @file    PortCollection.hpp
 * @author  Alessandro Thea
 * @date    20.08.2014
 */

#ifndef __SWATCH_PROCESSOR_PORTCOLLECTION_HPP__
#define __SWATCH_PROCESSOR_PORTCOLLECTION_HPP__


// SWATCH headers
#include "swatch/core/MonitorableObject.hpp"


namespace swatch {
namespace processor {

class InputPort;
class OutputPort;

//! Abstract class defining the optical link component interface of a processor
 class PortCollection : public core::MonitorableObject {
public:
    PortCollection();

    virtual ~PortCollection();

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

    const std::deque<InputPort*>& getInputs();

    const std::deque<OutputPort*>& getOutputs();

    InputPort& getInput( const std::string& aId );
    OutputPort& getOutput( const std::string& aId );

    typedef std::deque<InputPort*> tInputPortDeque;
    typedef std::deque<OutputPort*> tOutputPortDeque;

//protected:

    void addInput( InputPort* aInput );
    void addOutput( OutputPort*  aOutput );
    
protected:
    void retrieveMetricValues() {}

private:
    tInputPortDeque inputs_;
    tOutputPortDeque outputs_;
};

}
}

#endif	/* __SWATCH_PROCESSOR_PORTCOLLECTION_HPP__ */

