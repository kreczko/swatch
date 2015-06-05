/**
 * @file    XReferenceSet.hpp
 * @author  Alessandro Thea
 * @brief   Generic contailer for xdata::Serializable objects
 * @date    February 2015
 *
 */

#ifndef __SWATCH_CORE_XREFERENCESET_HPP__
#define __SWATCH_CORE_XREFERENCESET_HPP__

// C++ Headers
#include <typeinfo>

// Boost Headers
#include <boost/unordered_map.hpp>

#include <boost/mpl/assert.hpp>

// XDAQ Headers
#include <xdata/Serializable.h>

// Swatch Headers
#include "swatch/core/exception.hpp"
#include "swatch/core/XParameterSet.hpp"


namespace swatch {
namespace core {
class XReferenceSet;
}
}


namespace swatch {
namespace core {

class XReferenceSet {
public:

  XReferenceSet();
  virtual ~XReferenceSet();

  /**
   * List of parameters names stored.
   * @return vector of parameter names
   */
  std::set<std::string> keys() const;

  /**
   * Size of the set
   * @return Return container size
   */
  size_t size() const;

  /**
   * Count elements with a specific key
   * 
   * @param aName Reference aName
   * @return Number of occurrencies of "aName"
   */
  bool has( const std::string& aName ) const;

  void add( const std::string& aName , XParameterSet& aData );

  void add( const std::string& aName , const std::string& aParameterName , XParameterSet& aData );

 
  xdata::Serializable& get( const std::string& aName );

  const xdata::Serializable& get( const std::string& aName ) const;

  xdata::Serializable& operator[]( const std::string& aName );

  const xdata::Serializable& operator[]( const std::string& aName ) const;

  template<typename T>
  T& get( const std::string& aName );

  template<typename T>
  T& get( const std::string& aName ) const;

  typedef boost::unordered_map< std::string , xdata::Serializable* > EntryMap;

  //! Map storing the values
  EntryMap mEntries;

};

// Exceptions
DEFINE_SWATCH_EXCEPTION(XReferenceNotFound);
DEFINE_SWATCH_EXCEPTION(XReferenceExists);
DEFINE_SWATCH_EXCEPTION(XReferenceFailedCast);

} // namespace core
} // namespace swatch

#include "swatch/core/XReferenceSet.hxx"

#endif /* __SWATCH_CORE_XREFERENCESET_HPP__ */
