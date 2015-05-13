/**
 * @file    MP7Operations.hpp
 * @author  Alessandro Thea
 * @data    MArch 2015
 * 
 */

#ifndef __SWATCH_HARDWARE_MP7OPERATIONS_HPP__
#define __SWATCH_HARDWARE_MP7OPERATIONS_HPP__

#include "swatch/core/Operation.hpp"


namespace swatch {
namespace hardware {
 
class MP7Configure : public swatch::core::Operation {
   public:
       MP7Configure( swatch::core::ActionableObject* resource );
       virtual ~MP7Configure();

       bool pass();

       void coldReset();
       void configure();
       void enable();
       void suspend();
       void stop();

   };   
}
} // namespace swatch

#endif /* __SWATCH_HARDWARE_MP7OPERATIONS_HPP__ */