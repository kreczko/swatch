/* 
 * File:   Device.cpp
 * Author: ale
 */

#include "swatch/processor/LinkInterface.hpp"


// C++ headers
#include <iostream>

// SWATCH headers
#include "swatch/processor/Port.hpp"


namespace swatch {
namespace processor {

LinkInterface::LinkInterface():
core::MonitorableObject( "links" )
{
}


LinkInterface::~LinkInterface() {
}


void LinkInterface::addInput(InputPort* aInput) {
    this->addObj(aInput);
    inputs_.push_back(aInput);
}


void LinkInterface::addOutput(OutputPort* aOutput) {
    this->addObj(aOutput);
    outputs_.push_back(aOutput);
}


uint32_t LinkInterface::getNumInputs() const { return inputs_.size(); }

uint32_t LinkInterface::getNumOutputs() const { return outputs_.size(); }

const std::deque<InputPort*>& LinkInterface::getInputs() { return inputs_; }

const std::deque<OutputPort*>& LinkInterface::getOutputs() { return outputs_; }


InputPort& LinkInterface::getInput( const std::string& aId )
{
  if (InputPort* in = getObj<InputPort>( aId ))
    return *in;
  else
    throw std::runtime_error("LinkInterface \"" + this->getPath() + "\" does not contain any input port of ID \"" + aId + "\"");
}


OutputPort& LinkInterface::getOutput( const std::string& aId )
{
  if (OutputPort* out = getObj<OutputPort>( aId ))
    return *out;
  else
    throw std::runtime_error("LinkInterface \"" + this->getPath() + "\" does not contain any output port of ID \"" + aId + "\"");
}


} // end ns core
} // end ns swatch
