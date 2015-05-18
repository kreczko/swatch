/**
 * @file    Functionoid.hpp
 * @author  arose
 * @brief   
 * @date    May 2015
 *
 */

#ifndef __SWATCH_CORE_Functionoid_HPP__
#define __SWATCH_CORE_Functionoid_HPP__

#include "swatch/core/Object.hpp"

#include <deque>
#include <set>


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

    XParameterSet& getParams();

  protected:
    Object* getParent( const uint32_t& aDepth=1 );
    template<typename T> T* getParent( const uint32_t& aDepth=1 );
  
  private:
		XParameterSet mParams;
  };

} /* namespace core */
} /* namespace swatch */

#include "swatch/core/Functionoid.hxx"

#endif /* __SWATCH_CORE_Functionoid_HPP__ */
