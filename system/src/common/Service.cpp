/* 
 * File:   Service.cpp
 * Author: ale
 * Date: July 2014
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
