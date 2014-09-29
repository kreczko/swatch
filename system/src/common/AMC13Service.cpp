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
    slot_ = params.get<int>("slot",13);
    crateId_ = params.get<std::string>("crate","");
}


AMC13Service::~AMC13Service() {
}

uint32_t
AMC13Service::getSlot() const {
    return slot_;
}

const std::string&
AMC13Service::getCrateId() const {
    return crateId_;
}


} // namespace system
} // namespace swatch

