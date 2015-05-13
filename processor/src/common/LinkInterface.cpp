/* 
 * File:   Device.cpp
 * Author: ale
 * 
 * Created on July 11, 2014, 12:51 PM
 */

#include "swatch/processor/LinkInterface.hpp"
#include "swatch/processor/Port.hpp"

#include <boost/foreach.hpp>

using namespace std;

namespace swatch {
namespace processor {

LinkInterface::LinkInterface()
{
}

LinkInterface::~LinkInterface() {
  BOOST_FOREACH( tInputPortDeque::value_type p, inputs_) {
    delete p;
  }
  inputs_.clear();

  BOOST_FOREACH( tOutputPortDeque::value_type p, outputs_) {
    delete p;
  }
  outputs_.clear();
}

void
LinkInterface::addInput(InputPort* aInput) {
    inputs_.push_back(aInput);
}

void
LinkInterface::addOutput(OutputPort* aOutput) {
    outputs_.push_back(aOutput);
}


} // end ns core
} // end ns swatch
