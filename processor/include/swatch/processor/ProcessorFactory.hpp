/* 
 * File:   ProcessorFactory.hpp
 * Author: ale
 *
 * Created on August 5, 2014, 2:08 PM
 */

#ifndef __swatch_processor_servicefactory_hpp__
#define	__swatch_processor_servicefactory_hpp__


#include "swatch/core/AbstractFactory.hpp"
#include "swatch/processor/Processor.hpp"

namespace swatch {
namespace processor {

typedef swatch::core::AbstractFactory<swatch::processor::Processor> ProcessorFactory;

} // namespace processor
} // namespace swatch

#define SWATCH_PROCESSOR_REGISTER_CLASS( classname ) \
swatch::core::ClassRegistrationHelper< swatch::processor::Processor, classname > classname##ObjClassRegistrationHelper( #classname );

#define SWATCH_PROCESSOR_REGISTER_CREATOR( creatorname ) \
swatch::core::CreatorRegistrationHelper< swatch::processor::Processor, creatorname > creatorname##ObjCreatorRegistrationHelper( #creatorname );


#endif	/* __swatch_processor_servicefactory_hpp__ */


