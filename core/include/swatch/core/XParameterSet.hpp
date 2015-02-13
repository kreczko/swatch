/**
 * @file    XParameterSet.hpp
 * @author  Alessandro Thea
 * @brief   Device, class representing the trigger network node.
 * @date    February 2015
 *
 */
#ifndef __SWATCH_CORE_XPARAMETERSET_HPP__
#define __SWATCH_CORE_XPARAMETERSET_HPP__

// C++ Headers
#include <typeinfo>

// Boost Headers
#include <boost/type_traits/is_base_of.hpp>
#include <boost/unordered_map.hpp>

#include <boost/mpl/assert.hpp>

// XDAQ Headers
#include <xdata/Serializable.h>

// Swatch Headers
#include "swatch/core/exception.hpp"

namespace swatch {
namespace core {
      
class XParameterSet : public xdata::Serializable {
public:

  // class pair {
  // public:
  //   pair( const std::string& key, xdata::Serializable& value ) :
  //   key_(key), value_(value)
  //   {
  //   }
  //   const std::string& key() const {
  //     return key_;
  //   }

  //   xdata::Serializable& value() {
  //     return value_;
  //   }

  //   template<typename T>
  //   T& value() {
  //     return dynamic_cast<T&>(value_);
  //   }

  // private:
  //   std::string key_;
  //   xdata::Serializable& value_;
  // };



  // template<typename T> 
  // class iterator_base : public std::iterator< std::forward_iterator_tag , T >{
  //   friend class XParameterSet;

  //   iterator();
  //   virtual ~iterator();
  //   iterator_base ( T* aBegin );
  //   iterator_base ( iterator_base& aOrig );
  // public:
  // private:
  //   XParameterSet::EntryMap::iterator iter_;
  // };

  // typedef iterator_base<XParameterSet> iterator;
  // typedef iterator_base<const XParameterSet> const_iterator;


  XParameterSet();
  XParameterSet(const XParameterSet& orig);
  virtual ~XParameterSet();

  
  XParameterSet& operator=( const XParameterSet& );
  
  /**
   * Clear content
   */
  void clear();

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
   * @param name Parameter name
   * @return Number of occurrencies of "name"
   */
  bool has( const std::string& name ) const;


  virtual std::string type() const;
  virtual void setValue(const xdata::Serializable& s);
  virtual int equals(const xdata::Serializable&); // To be implemented
  virtual std::string toString() throw (xdata::exception::Exception);
  virtual void fromString(const std::string&) throw (xdata::exception::Exception);

  /**
   * Imports a parameter in the set. 
   * The parameter set takes the ownership of the pointer. The pointer must be a derived from xdata::Serializable
   * 
   * @param name Parameter name
   * @param data [description]
   */
  template<typename T>
  void adopt( const std::string& name , T* data );

  template<typename T>
  void set( const std::string& name , const T& data );

  template<typename T>
  T* pop( const std::string& name );  
  
  xdata::Serializable* pop( const std::string& name );
  
  void erase( const std::string& name );
  
  xdata::Serializable& get( const std::string& name );

  const xdata::Serializable& get( const std::string& name ) const;

  xdata::Serializable& operator[]( const std::string& name );

  const xdata::Serializable& operator[]( const std::string& name ) const;

  template<typename T>
  T& get( const std::string& name );

  template<typename T>
  T& get( const std::string& name ) const;

  void update( const std::string& name, const xdata::Serializable& data );
  

  class Inserter {
  public:

    template<typename T>
    Inserter& operator()(const std::string& aKey, const T& aValue);
    
  private:

    Inserter(XParameterSet* ps);

    XParameterSet *xps_;

    friend class XParameterSet;

  };

  template<typename T>
  Inserter insert(const std::string& aKey, const T& aValue);

  
private:

  template<typename T>
  static xdata::Serializable* cloner_( const xdata::Serializable* other );

  typedef xdata::Serializable* (*XCloner)( const xdata::Serializable* );

  struct XEntry {

    /**
     * Standard constructor
     *  
     * @param t Type info pointer
     * @param c Cloner function pointer
     * @param s Data to store in the entry
     */
    XEntry( const std::type_info* t, XCloner c, xdata::Serializable* s );
    
    /**
     * Copy constructor
     * The object pointer is not copied but a nuew istance is created instead.
     * @param orig Entry to copy from
     */
    XEntry( const XEntry& orig );

    bool operator==(const XEntry &other) const;

    // ! 
    const std::type_info* typeinfo;

    //!
    xdata::Serializable* (*cloner)( const xdata::Serializable* );

    //!
    xdata::Serializable* object;
  };
  
  typedef boost::unordered_map<std::string, XEntry> EntryMap;
  EntryMap entries_;

};

// Exceptions
DEFINE_SWATCH_EXCEPTION(XParameterNotFound);
DEFINE_SWATCH_EXCEPTION(XParameterExists);
DEFINE_SWATCH_EXCEPTION(XParameterFailedCast);

} // namespace core
} // namespace swatch

#include "swatch/core/XParameterSet.hxx"

#endif /* __SWATCH_CORE_XPARAMETERSET_HPP__ */