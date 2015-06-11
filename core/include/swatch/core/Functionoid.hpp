/**
 * @file    Functionoid.hpp
 * @author  arose
 * @brief   
 * @date    May 2015
 *
 */

#ifndef __SWATCH_CORE_Functionoid_HPP__
#define __SWATCH_CORE_Functionoid_HPP__

// C++ Headers
#include <deque>
#include <set>

// XDAQ Headers
#include <xdata/Serializable.h>

// Swatch Headers
#include "swatch/core/Object.hpp"


//Forward declarations
namespace swatch {
  namespace processor {
    class Processor;
  }
  namespace system {
    class System;
  }
}

namespace swatch {
namespace core {

  class Functionoid : public Object {
  public:
    /// Constructor
    Functionoid( const std::string& aId );

    /// Destructor
    virtual ~Functionoid();

  protected:
    Object* getParent( const uint32_t& aDepth=1 );
    template<typename T> T* getParent( const uint32_t& aDepth=1 );		

};

} /* namespace core */
} /* namespace swatch */

#include "swatch/core/Functionoid.hxx"

#endif /* __SWATCH_CORE_Functionoid_HPP__ */
