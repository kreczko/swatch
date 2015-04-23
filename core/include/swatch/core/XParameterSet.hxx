/**
 * @file    XParameterSet.hxx
 * @author  Alessandro Thea
 * @brief   Generic contailer for xdata::Serializable objects
 * @date    February 2015
 *
 */

#ifndef __SWATCH_CORE_XPARAMETERSET_HXX__
#define __SWATCH_CORE_XPARAMETERSET_HXX__

#include <boost/type_traits/is_base_of.hpp>

// Swatch Headers
//#include "swatch/core/checkgccversion.hpp"
#include "swatch/core/XParameterSet.hpp"
#include "swatch/core/Utilities.hpp"


namespace swatch {
namespace core {


//---
template<typename T>
void XParameterSet::adopt( const std::string& name , T* data ) {
    BOOST_STATIC_ASSERT( (boost::is_base_of<xdata::Serializable,T>::value) ); 
    /*
    if ( entries_.count(name) ) {
      throw XParameterExists(name + " is already defined");
    }

    // Uncomment emplace when XDAQ moves to C++0X
    // entries_.emplace( name, &typeid(T), static_cast<XCloner>(cloner_<T>), data );
    entries_[ name ] = XEntry(&typeid(T), static_cast<XCloner>(cloner_<T>), data) ;
    */
    
    // This is a dirty trick you should not look at. Needed because we cannot use emplace
    std::pair<EntryMap::iterator, bool> done = entries_.insert(std::make_pair(name, XEntry(&typeid(T), static_cast<XCloner>(cloner_<T>), static_cast<T*>(0x0))) );

    if ( not done.second ) {
        throw XParameterExists(name + " is already defined");
    }
    
    entries_.find(name)->second.object = data;
    
}


//---
template<typename T>
void
XParameterSet::add( const std::string& name , const T& data ) {
    BOOST_STATIC_ASSERT( (boost::is_base_of<xdata::Serializable,T>::value) ); 
    
    /*
    if ( entries_.count(name) ) {
      throw XParameterExists(name +" is already defined");
    }

    XCloner cloner = static_cast<XCloner>(cloner_<T>);
    // Uncomment emplace when XDAQ moves to C++0X
    // entries_.emplace( name, &typeid(T), cloner, static_cast<xdata::Serializable*>(cloner(&data)) );
    entries_[name] = XEntry(&typeid(T), cloner, static_cast<xdata::Serializable*>(cloner(&data)) );
    */
        
    XCloner cloner = static_cast<XCloner>(cloner_<T>);
    std::pair<EntryMap::iterator, bool> done = entries_.insert(std::make_pair(name, XEntry(&typeid(T), static_cast<XCloner>(cloner_<T>), cloner(&data))) );
    if ( not done.second ) {
        throw XParameterExists(name + " is already defined");
    }
}


//---
template<typename T>
T&
XParameterSet::get( const std::string& name ) {
     BOOST_STATIC_ASSERT( (boost::is_base_of<xdata::Serializable,T>::value) ); 
    return dynamic_cast<T&>( get(name) );
}

//---
template<typename T>
T&
XParameterSet::get( const std::string& name ) const {
     BOOST_STATIC_ASSERT( (boost::is_base_of<xdata::Serializable,T>::value) ); 
    return dynamic_cast<T&>( const_cast<xdata::Serializable&>(get(name)) );
}

//---
template<typename T>
T* XParameterSet::pop( const std::string& name ) {
  BOOST_STATIC_ASSERT( (boost::is_base_of<xdata::Serializable,T>::value) ); 

  EntryMap::iterator entryIt = entries_.find(name);

  if ( entryIt == entries_.end() ) {
      throw XParameterNotFound(name +" not found");
  }

  xdata::Serializable* s = entryIt->second.object;
  T* t = dynamic_cast<T*>(s);

  // If cast fails, throw
  if ( s != 0x0 and t == 0x0 ) {
    throw XParameterFailedCast("Failed to cast "+name+" to "+demangleName(entryIt->second.typeinfo->name() ) );
  }

  entries_.erase(entryIt);
  return t;
}

//---
template<typename T>
xdata::Serializable*
XParameterSet::cloner_( const xdata::Serializable* other ) {
  const T* xother = dynamic_cast<const T*>(other); 
  return new T(*xother);
}


//---
template<typename T>
XParameterSet::Inserter&
XParameterSet::Inserter::operator()(const std::string& aKey, const T& aValue) {
    BOOST_STATIC_ASSERT( (boost::is_base_of<xdata::Serializable,T>::value) ); 
    xps_->add(aKey, aValue);
    return *this;
}


//---
template<typename T>
XParameterSet::Inserter
XParameterSet::insert(const std::string& aKey, const T& aValue) {
  BOOST_STATIC_ASSERT( (boost::is_base_of<xdata::Serializable,T>::value) ); 
    this->add(aKey, aValue);
    return Inserter(this);
}



} // namespace core
} // namespace swatch


#endif /* __SWATCH_CORE_XPARAMETERSET_HXX__ */