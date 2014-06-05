// SWATCH HEADERS
#include "swatch/core/Link.hpp"

#include "swatch/core/Device.hpp"
#include "swatch/core/Port.hpp"


// OTHER HEADERS

// C++ HEADERS
#include <iostream>

using namespace swatch::core;
using namespace std;


Link::Link(
		const string& id,
		const boost::shared_ptr<Port>& src,
		const boost::shared_ptr<Port>& dst
		)
:
		id_(id),
		enabled_(false),
		srcPort_(src),
		dstPort_(dst)
{
	cout << "swatch::core::Link::Link --> Link CTOR called. Id " << id_ << endl;
}



Link::~Link()
{
	cout << "swatch::core::Link::~Link --> Link DTOR called. Id " << id_ << endl;
}


string
Link::getId()
{
	return id_;
}


boost::shared_ptr<Port>
Link::getSrcPort()
{
	return srcPort_;
}


boost::shared_ptr<Port>
Link::getDstPort()
{
	return dstPort_;
}


void
Link::enable()
{
	enabled_ = true;
}



bool
Link::isEnabled()
{
	return enabled_;
}
