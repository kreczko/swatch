/**
 * @file    ReadWriteXParameterSet.hxx
 * @author  Tom Williams, Alessandro Thea
 * @brief   Generic container for xdata::Serializable objects
 * @date    June 2015
 *
 */

#ifndef __SWATCH_CORE_READWRITEXPARAMETERSET_HXX__
#define __SWATCH_CORE_READWRITEXPARAMETERSET_HXX__


// boost headers
#include <boost/type_traits/is_base_of.hpp>

// SWATCH headers
#include "swatch/core/ReadWriteXParameterSet.hpp"
#include "swatch/core/Utilities.hpp"


namespace swatch {
namespace core {


//---
template<typename T>
void ReadWriteXParameterSet::adopt( const std::string& name , const boost::shared_ptr<T>& data ) {
    BOOST_STATIC_ASSERT( (boost::is_base_of<xdata::Serializable,T>::value) ); 
    /*
    if ( entries_.count(name) ) {
      throw XParameterExists(name + " is already defined");
    }

    // Uncomment emplace when XDAQ moves to C++0X
    // entries_.emplace( name, &typeid(T), static_cast<XCloner>(cloner_<T>), data );
    entries_[ name ] = XEntry(&typeid(T), static_cast<XCloner>(cloner_<T>), data) ;
    */
    std::pair<EntryMap_t::iterator, bool> it = emplace( name, &typeid(T), static_cast<XCloner_t>(clone<T>), boost::shared_ptr<xdata::Serializable>(data) );
    // Throw if failed to emplace
    if ( !it.second )
      throw XParameterExists(name + " is already defined");
   
    //C+98 compliant
    /*
    // This is a dirty trick you should not look at. Needed because we cannot use emplace
    std::pair<EntryMap::iterator, bool> done = entries_.insert(std::make_pair(name, XEntry(&typeid(T), static_cast<XCloner>(cloner_<T>), static_cast<T*>(0x0))) );

    if ( not done.second ) {
        throw XParameterExists(name + " is already defined");
    }
    
    entries_.find(name)->second.object = data;
    */
}


//---
template<typename T>
void ReadWriteXParameterSet::add( const std::string& name , const T& data ) {
    BOOST_STATIC_ASSERT( (boost::is_base_of<xdata::Serializable,T>::value) ); 
    
    XCloner_t cloner = static_cast<XCloner_t>(clone<T>);
    T* clone = static_cast<T*>(cloner(&data));

    adopt(name, boost::shared_ptr<T>(clone));
}


//---
template<typename T>
const T& ReadWriteXParameterSet::get( const std::string& name ) const
{ 
  return this->XParameterSet::get<T>(name);
}


//---
template<typename T>
xdata::Serializable* ReadWriteXParameterSet::clone( const xdata::Serializable* other ) {
  const T* xother = dynamic_cast<const T*>(other); 
  return new T(*xother);
}


} // namespace core
} // namespace swatch


#endif /* __SWATCH_CORE_READWRITEXPARAMETERSET_HXX__ */
