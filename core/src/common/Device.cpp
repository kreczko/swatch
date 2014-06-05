// SWATCH HEADERS
#include "swatch/core/Device.hpp"


// OTHER HEADERS

// C++ HEADERS
#include <iostream>


using namespace swatch::core;
using namespace std;



Device::Device(const string& id):
		id_(id),
		state_(Halted)
{
	cout << "swatch::core::Device::Device --> Device CTOR called. Id: " << id_ << endl;
}



Device::~Device()
{
	cout << "swatch::core::Device::~Device --> Device DTOR called. Id " << id_ << endl;
}



string
Device::getId()
{
	return id_;
}


void
Device::addTx(const Device::shared_port& tx)
{
	txPorts_.push_back(tx);
}


void
Device::addRx(const Device::shared_port& rx)
{
	txPorts_.push_back(rx);
}


void
Device::configure()
{
}


void
Device::enable()
{
}


void
Device::suspend()
{
}


void
Device::stop()
{

}


void
Device::resume()
{
}


void
Device::test()
{
}


void
Device::setState(const Device::FSMStates& state)
{
	state_ = state;
}


Device::FSMStates
Device::getState()
{
	return state_;
}
