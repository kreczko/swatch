/* 
 * File:   Service.cpp
 * Author: ale
 * 
 * Created on July 21, 2014, 10:16 AM
 */

#include "swatch/system/Service.hpp"

namespace swatch {
namespace system {


Service::Service( const swatch::core::AbstractStub& aStub ) : core::Object(aStub.id) {
}

Service::~Service() {
}

}
}
