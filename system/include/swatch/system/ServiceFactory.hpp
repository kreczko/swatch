/* 
 * File:   ServiceFactory.hpp
 * Author: ale
 *
 * Created on August 3, 2014, 7:42 PM
 */

#ifndef __swatch_system_servicefactory_hpp__
#define	__swatch_system_servicefactory_hpp__


#include "swatch/core/AbstractFactory.hpp"

namespace swatch {
namespace system {

typedef swatch::core::AbstractFactory<swatch::system::Service> ServiceFactory;

} // namespace system
} // namespace swatch

#define SWATCH_SERVICE_REGISTER_CLASS( classname ) \
swatch::core::ClassRegistrationHelper< swatch::system::Service, classname > classname##ObjClassRegistrationHelper( #classname );

#define SWATCH_SERVICE_REGISTER_CREATOR( creatorname ) \
swatch::core::CreatorRegistrationHelper< swatch::system::Service, creatorname > creatorname##ObjCreatorRegistrationHelper( #creatorname );


#endif	/* __swatch_system_servicefactory_hpp__ */

