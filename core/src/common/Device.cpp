/* 
 * File:   Device.cpp
 * Author: ale
 * 
 * Created on July 11, 2014, 12:51 PM
 */

#include "swatch/core/Device.hpp"
#include "swatch/core/Port.hpp"

using namespace std;

namespace swatch {
namespace core {

Device::Device(const std::string& aId, const ParameterSet& params) :
    Object(aId, params),
    fsm_(FsmStates::HALTED)
{
}

Device::~Device() {
}

void
Device::addInput(InputPort* aInput) {
    this->addObj(aInput);
    inputs_.push_back(aInput);
}

void
Device::addOutput(OutputPort* aOutput) {
    this->addObj(aOutput);
    outputs_.push_back(aOutput);
}

void
Device::halt(const ParameterSet& params)
{
	ostringstream msg;

	// lock
	// FSM check

	if (!canHalt())
	{
		msg << "c_halt check failed! Could not finish transition to HALTED state from " << fsm_ << endl;
		throw runtime_error(msg.str());
	}

	doHalt();

	fsm_ = Device::FsmStates::HALTED;
	msg << "FSM set to HALTED" << endl;
}

void
Device::configure(const ParameterSet& params)
{
	ostringstream msg;

	// lock

	// FSM check
	if (fsm_ != Device::FsmStates::HALTED)
	{
		msg << "Transition from state " << fsm_ << " to CONFIGURED not allowed!" << endl;
		throw runtime_error(msg.str());
	}

	if (!canConfigure())
	{
		msg << "c_configure check failed! Could not finish transition to CONFIGURED state from " << fsm_ << endl;
		throw runtime_error(msg.str());
	}

	canConfigure();

	fsm_ = Device::FsmStates::CONFIGURED;
	msg << "FSM set to CONFIGURED" << endl;

}


bool
Device::canHalt()
{
	return true;
}


void
Device::doHalt(const ParameterSet& params)
{

}

bool
Device::canConfigure()
{
	return true;
}

void
Device::doConfigure(const ParameterSet& params)
{

}

} // end ns core
} // end ns swatch
