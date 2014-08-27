/* 
 * File:   Service.cpp
 * Author: ale
 * 
 * Created on July 21, 2014, 10:16 AM
 */

#include "swatch/system/Service.hpp"

namespace swatch {
namespace system {


Service::Service( const std::string &aId, const core::Arguments& aArguments ) : core::Object(aId, aArguments) {
}

Service::~Service() {
}

}
}
