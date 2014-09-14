/* 
 * File:   SystemFActory.hpp
 * Author: ale
 *
 * Created on August 1, 2014, 9:16 AM
 */

#ifndef __swatch_system_systemfactory_hpp__
#define	__swatch_system_systemfactory_hpp__

#include "swatch/core/AbstractFactory.hpp"
#include "swatch/system/System.hpp"


namespace swatch {
namespace system {

typedef swatch::core::AbstractFactory<swatch::system::System> SystemFactory;

} // namespace system
} // namespace swatch

#define SWATCH_SYSTEM_REGISTER_CLASS( classname ) \
swatch::core::ClassRegistrationHelper< swatch::system::System, classname > classname##ObjClassRegistrationHelper( #classname );

#define SWATCH_SYSTEM_REGISTER_CREATOR( creatorname ) \
swatch::core::CreatorRegistrationHelper< swatch::system::System, creatorname > creatorname##ObjCreatorRegistrationHelper( #creatorname );

#endif	/* __swatch_system_systemfactory_hpp__ */

