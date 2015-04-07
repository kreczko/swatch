/* 
 * File:   ServiceFactory.hpp
 * Author: ale
 *
 * Created on August 3, 2014, 7:42 PM
 */

#ifndef __SWATCH_SYSTEM_SERVICEFACTORY_HPP__
#define	__SWATCH_SYSTEM_SERVICEFACTORY_HPP__


#include "swatch/core/AbstractFactory.hpp"
#include "swatch/system/Service.hpp"

namespace swatch {
namespace system {

typedef swatch::core::AbstractFactory<swatch::system::Service> ServiceFactory;

} // namespace system
} // namespace swatch

#define SWATCH_SERVICE_REGISTER_CLASS( classname ) _SWATCH_ABSTRACT_REGISTER_CLASS( swatch::system::Service, classname )
#define SWATCH_SERVICE_REGISTER_CREATOR( creatorname ) _SWATCH_ABSTRACT_REGISTER_CREATOR( swatch::system::Service, creatorname )

#endif	/* __SWATCH_SYSTEM_SERVICEFACTORY_HPP__ */

