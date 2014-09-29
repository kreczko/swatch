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
Processor::Processor( const std::string& aId, const core::ParameterSet& params ) :
    Device(aId, params) {
    
}

Processor::~Processor() {
}


void
Processor::halt(const core::ParameterSet& params)
{
	if (!c_halt())
	{

	}

	f_halt(params);
}

void
Processor::configure(const core::ParameterSet& params)
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
Processor::f_halt(const core::ParameterSet& params)
{

}

bool
Processor::c_configure()
{
	return true;
}

void
Processor::f_configure(const core::ParameterSet& params)
{

}


} // namespace processor
} // namespace swatch

