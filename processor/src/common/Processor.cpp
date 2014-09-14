/* 
 * File:   Processor.cpp
 * Author: ale
 * 
 * Created on July 11, 2014, 10:55 AM
 */


#include "swatch/processor/Processor.hpp"

// Swatch Headers
#include "swatch/core/Utilities.hpp"
#include "swatch/core/Device.hpp"

// Boost Headers
#include <boost/detail/container_fwd.hpp>
#include <boost/unordered/unordered_map.hpp>
#include "boost/foreach.hpp"


using namespace std;

namespace swatch {
namespace processor {

const uint32_t Processor::NoSlot =  0x7fffffffL;

///---
Processor::Processor( const std::string& aId, const core::Arguments& aArguments ) :
    Device(aId, aArguments) {
    
}

Processor::~Processor() {
}


void
Processor::halt(const core::Arguments& params)
{
	if (!c_halt())
	{

	}

	f_halt(params);
}

void
Processor::configure(const core::Arguments& params)
{
	if (!c_configure())
	{

	}

	f_configure(params);
}

bool
Processor::c_halt()
{
	return true;
}

void
Processor::f_halt(const core::Arguments& params)
{

}

bool
Processor::c_configure()
{
	return true;
}

void
Processor::f_configure(const core::Arguments& params)
{

}


} // namespace processor
} // namespace swatch

