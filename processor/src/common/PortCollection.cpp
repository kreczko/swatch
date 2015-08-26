
#include "swatch/processor/PortCollection.hpp"


// C++ headers
#include <iostream>

// SWATCH headers
#include "swatch/processor/Port.hpp"


namespace swatch {
namespace processor {

PortCollection::PortCollection() :
  core::MonitorableObject( "ports" )
{
}


PortCollection::~PortCollection() {
}


void PortCollection::addInput(InputPort* aInput) {
    this->addObj(aInput);
    inputs_.push_back(aInput);
}


void PortCollection::addOutput(OutputPort* aOutput) {
    this->addObj(aOutput);
    outputs_.push_back(aOutput);
}


uint32_t PortCollection::getNumInputs() const { return inputs_.size(); }

uint32_t PortCollection::getNumOutputs() const { return outputs_.size(); }

const std::deque<InputPort*>& PortCollection::getInputs() { return inputs_; }

const std::deque<OutputPort*>& PortCollection::getOutputs() { return outputs_; }


InputPort& PortCollection::getInput( const std::string& aId )
{
  if (InputPort* in = getObj<InputPort>( aId ))
    return *in;
  else
    throw std::runtime_error("PortCollection \"" + this->getPath() + "\" does not contain any input port of ID \"" + aId + "\"");
}


OutputPort& PortCollection::getOutput( const std::string& aId )
{
  if (OutputPort* out = getObj<OutputPort>( aId ))
    return *out;
  else
    throw std::runtime_error("PortCollection \"" + this->getPath() + "\" does not contain any output port of ID \"" + aId + "\"");
}


} // end ns core
} // end ns swatch
