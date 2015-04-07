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

#define SWATCH_SYSTEM_REGISTER_CLASS( classname ) _SWATCH_ABSTRACT_REGISTER_CLASS( swatch::system::System, classname)
#define SWATCH_SYSTEM_REGISTER_CREATOR( creatorname ) _SWATCH_ABSTRACT_REGISTER_CREATOR( swatch::system::System, creatorname)

#endif	/* __SWATCH_SYSTEM_SYSTEMFACTORY_HPP__ */

