/**
 * @file    Placeholder.hpp
 * @author  Andy Rose
 * @brief   Implementation of the templated methods
 * @date    May 2015
 */

#ifndef __SWATCH_CORE_Placeholder_HPP__
#define	__SWATCH_CORE_Placeholder_HPP__

// XDAQ Headers
#include "xdata/Serializable.h"

// Boost Headers
#include <boost/type_traits/is_base_of.hpp>
#include <boost/static_assert.hpp>

namespace swatch {
namespace core {

template<typename T>
class Placeholder : public xdata::Serializable {
  BOOST_STATIC_ASSERT( (boost::is_base_of<xdata::Serializable,T>::value) ); 

  public:
  Placeholder();
  virtual ~Placeholder();
 
  virtual std::string type() const;
  
  virtual void setValue(const xdata::Serializable& s);
  
  virtual int equals(const xdata::Serializable& s);
  
  virtual std::string toString() throw (xdata::exception::Exception);
  
  virtual void fromString(const std::string& s) throw (xdata::exception::Exception);  
};

}
}

#include "swatch/core/Placeholder.hxx"

#endif	/* __SWATCH_CORE_Placeholder_HPP__ */

