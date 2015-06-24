/**
 * @file    MP7Operations.hpp
 * @author  Alessandro Thea
 * @data    MArch 2015
 * 
 */

#ifndef __SWATCH_MP7_MP7OPERATIONS_HPP__
#define __SWATCH_MP7_MP7OPERATIONS_HPP__

#include "swatch/core/Operation.hpp"


namespace swatch {
namespace mp7 {
 
class MP7Configure : public swatch::core::Operation {
   public:
       MP7Configure(const std::string& aId);
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

#endif /* __SWATCH_MP7_MP7OPERATIONS_HPP__ */
