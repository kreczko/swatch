/**
 * @file    XPointer.hpp
 * @author  Alessandro Thea
 * @brief   Implementation of the templated methods
 * @date    April 2015
 */

#ifndef __SWATCH_CORE_XPOINTER_HPP__
#define	__SWATCH_CORE_XPOINTER_HPP__

// XDAQ Headers
#include "xdata/Serializable.h"

// Boost Headers
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/static_assert.hpp>

namespace swatch {
namespace core {

/*
 * @brief A xdata::Serializable object with the ownership semantics of a boost::shared_ptr. 
 * Can be used to add items to a XParameterSet without the XParameterSet actually owning the items. 
 */
template<typename T>
class XPointer : public xdata::Serializable {
  BOOST_STATIC_ASSERT( (boost::is_base_of<xdata::Serializable,T>::value) ); 

  public:
  XPointer();
  XPointer( T* ptr );
  XPointer(const XPointer& orig);
  virtual ~XPointer();

  void operator=( const XPointer<T>& b );

  bool operator==( const XPointer<T>& b ) const;

  long count() const;

  T& operator*();
  
  const T& operator*() const;
  
  T* operator->();

  const T* operator->() const;


  
  virtual std::string type() const;
  
  virtual void setValue(const xdata::Serializable& s);
  
  virtual int equals(const xdata::Serializable& s);
  
  virtual std::string toString() throw (xdata::exception::Exception);
  
  virtual void fromString(const std::string& s) throw (xdata::exception::Exception);



private:
  boost::shared_ptr<T> ptr_;
  
};

}
}

#include "swatch/core/XPointer.hxx"

#endif	/* __SWATCH_CORE_XPOINTER_HPP__ */

