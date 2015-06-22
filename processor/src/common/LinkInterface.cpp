/* 
 * File:   Device.cpp
 * Author: ale
 * 
 * Created on July 11, 2014, 12:51 PM
 */

#include "swatch/processor/LinkInterface.hpp"
#include "swatch/processor/Port.hpp"

#include <iostream>

using namespace std;

namespace swatch {
namespace processor {

LinkInterface::LinkInterface():
core::MonitorableObject( "links" )
{
}

LinkInterface::~LinkInterface() {
}

void
LinkInterface::addInput(InputPort* aInput) {
    this->addObj(aInput);
    inputs_.push_back(aInput);
    
}

void
LinkInterface::addOutput(OutputPort* aOutput) {
    this->addObj(aOutput);
    outputs_.push_back(aOutput);
}


uint32_t LinkInterface::getNumInputs() const { return inputs_.size(); }

uint32_t LinkInterface::getNumOutputs() const { return outputs_.size(); }

std::deque<InputPort*>& LinkInterface::getInputs() { return inputs_; }

std::deque<OutputPort*>& LinkInterface::getOutputs() { return outputs_; }


InputPort& LinkInterface::getInput( const std::string& aId )
{
  if (InputPort* in = getObj<InputPort>( aId ))
    return *in;
  else
    throw std::runtime_error("LinkInterface \""+this->path()+"\" does not contain any input port of ID \"" + aId + "\"");
}


OutputPort& LinkInterface::getOutput( const std::string& aId )
{
  if (OutputPort* out = getObj<OutputPort>( aId ))
    return *out;
  else
    throw std::runtime_error("LinkInterface \""+this->path()+"\" does not contain any output port of ID \"" + aId + "\"");
}


} // end ns core
} // end ns swatch
