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

// XDAQ Headers
#include <xdata/Serializable.h>


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

    const XParameterSet& getDefaultParams() const;

    /**
     * Merges a parameter set with the default parameter set.
     * Default values are only used if not present in params.
     */
    const XParameterSet mergeParametersWithDefaults(const XParameterSet& params) const;

  protected:
    Object* getParent( const uint32_t& aDepth=1 );
    template<typename T> T* getParent( const uint32_t& aDepth=1 );

  
  private:
		XParameterSet parameters_;

protected:
  template<typename T>
  void registerParameter(const std::string name,
      const T& defaultValue);
  };

} /* namespace core */
} /* namespace swatch */

#include "swatch/core/Functionoid.hxx"

#endif /* __SWATCH_CORE_Functionoid_HPP__ */
