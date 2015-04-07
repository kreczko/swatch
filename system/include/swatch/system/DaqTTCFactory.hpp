/* 
 * File:   ServiceFactory.hpp
 * Author: ale
 *
 * Created on August 3, 2014, 7:42 PM
 */

#ifndef __SWATCH_SYSTEM_DAQTTCFACTORY_HPP__
#define	__SWATCH_SYSTEM_DAQTTCFACTORY_HPP__


#include "swatch/core/AbstractFactory.hpp"
#include "swatch/system/DaqTTCService.hpp"

namespace swatch {
namespace system {

typedef swatch::core::AbstractFactory<swatch::system::DaqTTCService> DaqTTCFactory;

} // namespace system
} // namespace swatch

#define SWATCH_DAQTTC_REGISTER_CLASS( classname ) \
swatch::core::ClassRegistrationHelper< swatch::system::DaqTTCService, classname > classname##ObjClassRegistrationHelper( #classname );

#define SWATCH_DAQTTC_REGISTER_CREATOR( creatorname ) \
swatch::core::CreatorRegistrationHelper< swatch::system::DaqTTCService, creatorname > creatorname##ObjCreatorRegistrationHelper( #creatorname );


#endif	/* __SWATCH_SYSTEM_DAQTTCFACTORY_HPP__ */