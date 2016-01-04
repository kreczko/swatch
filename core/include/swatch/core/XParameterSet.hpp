/**
 * @file    XParameterSet.hpp
 * @author  Alessandro Thea
 * @date    February 2015
 */

#ifndef __SWATCH_CORE_XPARAMETERSET_HPP__
#define __SWATCH_CORE_XPARAMETERSET_HPP__


// Swatch Headers
#include "swatch/core/exception.hpp"


// Forward declarations
namespace xdata {
class Serializable;
}

namespace swatch {
namespace core {


class XParameterSet; 
    
std::ostream& operator<< ( std::ostream& aStr , const swatch::core::XParameterSet& aXParameterSet );


//! Generic container (read-only) interface for containers of xdata::Serializable objects   
class XParameterSet {
public:

  friend std::ostream& (operator<<) ( std::ostream& aStr , const swatch::core::XParameterSet& aXParameterSet );

  XParameterSet();

  XParameterSet(const XParameterSet& aOther);

  virtual ~XParameterSet();

  /**
   * Size of the collection
   * @return Number of stored parameters
   */
  virtual size_t size() const = 0;

  /**
   * List of names of stored parameters.
   * @return names of stored parameters
   */
  virtual std::set<std::string> keys() const = 0;

  //! Returns whether parameter with given name exists
  virtual bool has( const std::string& aName ) const = 0;

  //! Retrieve reference to parameter with specified name; throws XParameterNotFound if doesn't contain a parameter with specified name
  virtual const xdata::Serializable& get( const std::string& aName ) const = 0;

  //! Retrieve reference to parameter with specified name; throws XParameterNotFound if doesn't contain a parameter with specified name
  virtual const xdata::Serializable& operator[]( const std::string& aName ) const = 0;

  //! Retrieve reference to parameter with specified name; throws XParameterFailedCast if dynamic cast fails; throws XParameterNotFound if doesn't contain a parameter with specified name
  template<typename T>
  const T& get( const std::string& aName ) const;
  
  //! Retrieves string representation of parameter with specified name - i.e. the result of calling xdata::Serializable::toString() method; throws XParameterNotFound if doesn't contain a parameter with specified name
  virtual std::string parameterAsString(const std::string& aName) const = 0;

  /**
   * Removes parameter of specified name from the list of variables.
   * 
   * @param aName ParameterSet entry to delete
   */
  virtual void erase( const std::string& aName ) = 0;

};

// Exceptions
DEFINE_SWATCH_EXCEPTION(XParameterNotFound);
DEFINE_SWATCH_EXCEPTION(XParameterExists);
DEFINE_SWATCH_EXCEPTION(XParameterFailedCast);

} // namespace core
} // namespace swatch

#include "swatch/core/XParameterSet.hxx"

#endif /* __SWATCH_CORE_XPARAMETERSET_HPP__ */
