/* 
 * File:   AMC13.cpp
 * Author: ale
 * 
 * Created on July 28, 2014, 12:13 AM
 */

#include "swatch/system/DaqTTCService.hpp"

#include "swatch/system/ServiceFactory.hpp"


// Boost Headers
#include "boost/foreach.hpp"

// Namespace resolution
using namespace std;

namespace swatch {
namespace system {

DaqTTCService::DaqTTCService(const std::string& aId, const core::XParameterSet& aPars) : swatch::core::Object(aId, aPars) {

}


DaqTTCService::~DaqTTCService() {
}


} // namespace system
} // namespace swatch

