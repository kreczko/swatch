/**
 * @file    Functionoid.hpp
 * @author  arose
 * @brief   
 * @date    May 2015
 *
 */

#ifndef __SWATCH_CORE_FUNCTIONOID_HPP__
#define __SWATCH_CORE_FUNCTIONOID_HPP__


// XDAQ Headers
#include <xdata/Serializable.h>

// Swatch Headers
#include "swatch/core/Object.hpp"


namespace swatch {
namespace core {

class Functionoid : public Object {
public:
  /// Constructor
  Functionoid( const std::string& aId );

  /// Destructor
  virtual ~Functionoid();

protected:
  Object* getParent();

  template<typename T> T* getParent();
};


} /* namespace core */
} /* namespace swatch */

#include "swatch/core/Functionoid.hxx"

#endif /* __SWATCH_CORE_Functionoid_HPP__ */
