/* 
 * File:   Service.hpp
 * Author: ale
 *
 * Created on July 21, 2014, 10:16 AM
 */

#ifndef __SWATCH_SYSTEM_SERVICE_HPP__
#define	__SWATCH_SYSTEM_SERVICE_HPP__

#include "swatch/core/Object.hpp"

namespace swatch {
namespace system {

// just to fill the spot
class Service : public core::Object {
public:
    Service( const std::string &aId, const core::XParameterSet& aPars );
    virtual ~Service();
};
}
}

#endif	/* __SWATCH_SYSTEM_SERVICE_HPP__ */

