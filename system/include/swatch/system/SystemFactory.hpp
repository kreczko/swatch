/* 
 * File:   SystemFActory.hpp
 * Author: ale
 *
 * Created on August 1, 2014, 9:16 AM
 */

#ifndef __SWATCH_SYSTEM_SYSTEMFACTORY_HPP__
#define	__SWATCH_SYSTEM_SYSTEMFACTORY_HPP__

#include "swatch/core/AbstractFactory.hpp"
#include "swatch/system/System.hpp"


namespace swatch {
namespace system {

typedef swatch::core::AbstractFactory<swatch::system::System> SystemFactory;

} // namespace system
} // namespace swatch

// Standard factory registration macros
#define SWATCH_SYSTEM_REGISTER_CLASS( classname ) \
template<> bool swatch::core::ClassRegistrationHelper<swatch::system::System, classname >::initialised_= \
  swatch::core::ClassRegistrationHelper<swatch::system::System, classname >::init(#classname);

#define SWATCH_SYSTEM_REGISTER_CREATOR( creatorname ) \
template<> bool swatch::core::CreatorRegistrationHelper< swatch::system::System, creatorname >::initialised_= \
  swatch::core::CreatorRegistrationHelper< swatch::system::System, creatorname >::init(#creatorname);


#endif	/* __SWATCH_SYSTEM_SYSTEMFACTORY_HPP__ */

