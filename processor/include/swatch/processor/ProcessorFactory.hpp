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

// Standard factory registration macros
#define SWATCH_PROCESSOR_REGISTER_CLASS( classname ) \
template<> bool swatch::core::ClassRegistrationHelper< swatch::processor::Processor, classname >::initialised_= \
  swatch::core::ClassRegistrationHelper< swatch::processor::Processor, classname >::init(#classname);

#define SWATCH_PROCESSOR_REGISTER_CREATOR( creatorname ) \
template<> bool swatch::core::CreatorRegistrationHelper< swatch::processor::Processor, creatorname >::initialised_= \
  swatch::core::CreatorRegistrationHelper< swatch::processor::Processor, creatorname >::init(#creatorname);

#endif	/* __swatch_processor_servicefactory_hpp__ */


