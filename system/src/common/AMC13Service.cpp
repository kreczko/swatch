/* 
 * File:   AMC13.cpp
 * Author: ale
 * 
 * Created on July 28, 2014, 12:13 AM
 */

#include "swatch/system/AMC13Service.hpp"

#include "swatch/system/ServiceFactory.hpp"


// Boost Headers
#include "boost/foreach.hpp"

// Namespace resolution
using namespace std;

namespace swatch {
namespace system {

//SWATCH_SERVICE_REGISTER_CLASS(AMC13Service)

AMC13Service::AMC13Service(const std::string& aId, const core::ParameterSet& params) : Service(aId, params) {

}


AMC13Service::~AMC13Service() {
}


} // namespace system
} // namespace swatch

