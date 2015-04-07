/* 
 * File:   ProcessorFactory.hpp
 * Author: ale
 *
 * Created on August 5, 2014, 2:08 PM
 */

#ifndef __SWATCH_PROCESSOR_SERVICEFACTORY_HPP__
#define	__SWATCH_PROCESSOR_SERVICEFACTORY_HPP__


#include "swatch/core/AbstractFactory.hpp"
#include "swatch/processor/Processor.hpp"

namespace swatch {
namespace processor {

typedef swatch::core::AbstractFactory<swatch::processor::Processor> ProcessorFactory;

} // namespace processor
} // namespace swatch

// Processor Class Registrator
#define SWATCH_PROCESSOR_REGISTER_CLASS( classname ) _SWATCH_ABSTRACT_REGISTER_CLASS( swatch::processor::Processor, classname )
// Processor Creator Registrator
#define SWATCH_PROCESSOR_REGISTER_CREATOR( creatorname ) _SWATCH_ABSTRACT_REGISTER_CREATOR( swatch::processor::Processor, creatorname )


#endif	/* __SWATCH_PROCESSOR_SERVICEFACTORY_HPP__ */


