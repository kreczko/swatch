#ifndef __SWATCH_CORE_XPARAMETERSET_HXX__
#define __SWATCH_CORE_XPARAMETERSET_HXX__

#include "XParameterSet.hpp"
#include "Utilities.hpp"


namespace swatch {
namespace core {


//---
template<typename T>
void XParameterSet::adopt( const std::string& name , T* data ) {
    BOOST_STATIC_ASSERT( (boost::is_base_of<xdata::Serializable,T>::value) ); 

    if ( entries_.count(name) ) {
      throw XParameterExists(name + " is already defined");
    }

    entries_.emplace( name, &typeid(T), static_cast<XCloner>(cloner_<T>), data );

}


//---
template<typename T>
void
XParameterSet::add( const std::string& name , T data ) {
    BOOST_STATIC_ASSERT( (boost::is_base_of<xdata::Serializable,T>::value) ); 

    if ( entries_.count(name) ) {
      throw XParameterExists(name +" is already defined");
    }

    entries_.emplace( name, &typeid(T), static_cast<XCloner>(cloner_<T>), new T(data) );

}


//---
template<typename T>
T&
XParameterSet::get( const std::string& name ) {
    // EntryMap::iterator it = entries_.find(name);
    // if ( it == entries_.end() ) {
      // throw XParameterNotFound(name +" not found");
    // }
    
    // return dynamic_cast<T&>(*(it->second.object));
    return dynamic_cast<T&>( get(name) );
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