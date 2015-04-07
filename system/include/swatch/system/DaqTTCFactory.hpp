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


// Standard factory registration macros
#define SWATCH_DAQTTC_REGISTER_CLASS( classname ) \
template<> bool swatch::core::ClassRegistrationHelper2g<swatch::system::DaqTTCService, classname >::initialised_= \
  swatch::core::ClassRegistrationHelper2g<swatch::system::DaqTTCService, classname >::init(#classname);

#define SWATCH_DAQTTC_REGISTER_CREATOR( creatorname ) \
template<> bool swatch::core::CreatorRegistrationHelper2g< swatch::system::DaqTTCService, creatorname >::initialised_= \
  swatch::core::CreatorRegistrationHelper2g< swatch::system::DaqTTCService, creatorname >::init(#creatorname);


#endif	/* __SWATCH_SYSTEM_DAQTTCFACTORY_HPP__ */