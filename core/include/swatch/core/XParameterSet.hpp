/**
 * @file    XParameterSet.hpp
 * @author  Alessandro Thea
 * @brief   Generic contailer for xdata::Serializable objects
 * @date    February 2015
 *
 */

#ifndef __SWATCH_CORE_XPARAMETERSET_HPP__
#define __SWATCH_CORE_XPARAMETERSET_HPP__

// C++ Headers
#include <typeinfo>

// Boost Headers
#include <boost/unordered_map.hpp>

#include <boost/mpl/assert.hpp>

// XDAQ Headers
#include <xdata/Serializable.h>

// Swatch Headers
#include "swatch/core/exception.hpp"

namespace swatch {
namespace core {
      

//! Generic container for xdata::Serializable objects   
class XParameterSet : public xdata::Serializable {
public:

  XParameterSet();
  XParameterSet(const XParameterSet& orig);
  virtual ~XParameterSet();

  /**
   * Assignment operator
   */
  XParameterSet& operator=( const XParameterSet& );
  
  bool operator==( const XParameterSet& ) const;
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
   * Adopt a parameter in the set. 
   * The parameter set takes the ownership of the pointer. The pointer must be a derived from xdata::Serializable
   * 
   * @param name Parameter name
   * @param data Pointer to import in the set
   */
  template<typename T>
  void adopt( const std::string& name , T* data );

  /**
   * Add a parameter to the set, copying data into the data
   * 
   * @param name Parameter name
   * @param data Value to copy in the set.
   */
  template<typename T>
  void add( const std::string& name , const T& data );


  /**
   * Sets the value of a parameter in the set. The parameter must exist.
   * 
   * @param name Parameter name
   * @param data Value to copy in the set.
   */
  void set( const std::string& name , const xdata::Serializable& data );

  /**
   * Pop an entry from the set
   * 
   * @param name Parameter to pop.
   * @return Pointer to the entry removed.
   */
  template<typename T>
  T* pop( const std::string& name );  
  
  /**
   * Pops 'name' from the list of variable and 
   * 
   * @param name Variable to extract
   * @return Pointer to the popped variable
   */
  xdata::Serializable* pop( const std::string& name );
  
  /**
   * Erases 'name' from the list of variables.
   * 
   * @param name Variable to delete
   */
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
     * The object pointer is not copied but a new instance is created instead.
     * 
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
  
  std::pair<EntryMap::iterator,bool> emplace( const std::string& name, const std::type_info* t, XCloner c, xdata::Serializable* s );
  std::pair<EntryMap::iterator,bool> emplace( const std::string& name, const XEntry& );

  //! Map storing the values
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