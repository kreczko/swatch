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
  XParameterSet();
  XParameterSet(const XParameterSet& orig);
  virtual ~XParameterSet();

  XParameterSet& operator=( const XParameterSet& );
  
  void clear();
  std::set<std::string> names() const;
  size_t size() const;
  size_t count( const std::string& name ) const;

  virtual std::string type() const;
  virtual void setValue(const xdata::Serializable& s);
  virtual int equals(const xdata::Serializable&); // To be implemented
  virtual std::string toString() throw (xdata::exception::Exception);
  virtual void fromString(const std::string&) throw (xdata::exception::Exception);


  template<typename T>
  void adopt( const std::string& name , T* data );

  template<typename T>
  void add( const std::string& name , T data );

  template<typename T>
  T* pop( const std::string& name );  
  
  xdata::Serializable* pop( const std::string& name );
  
  void erase( const std::string& name );
  
  xdata::Serializable& get( const std::string& name );

  const xdata::Serializable& get( const std::string& name ) const;

  void set( const std::string& name, const xdata::Serializable& data );
  
  template<typename T>
  T& get( const std::string& name );
  
private:

  template<typename T>
  static xdata::Serializable* cloner_( const xdata::Serializable* other ) {
        const T* xother = dynamic_cast<const T*>(other); 
        return new T(*xother);
    }

  typedef xdata::Serializable* (*XCloner)( const xdata::Serializable* );

  struct XEntry {

    XEntry( const std::type_info* t, XCloner c, xdata::Serializable* s );
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