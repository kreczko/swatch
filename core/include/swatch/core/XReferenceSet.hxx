/**
 * @file    XReferenceSet.hxx
 * @author  Alessandro Thea
 * @brief   Generic container for xdata::Serializable objects
 * @date    February 2015
 *
 */

#ifndef __SWATCH_CORE_XREFERENCESET_HXX__
#define __SWATCH_CORE_XREFERENCESET_HXX__

#include <boost/type_traits/is_base_of.hpp>

// Swatch Headers
//#include "swatch/core/checkgccversion.hpp"
#include "swatch/core/XReferenceSet.hpp"
#include "swatch/core/Utilities.hpp"


namespace swatch {
namespace core {

//---
template<typename T>
T&
XReferenceSet::get( const std::string& aName ) {
    BOOST_STATIC_ASSERT( (boost::is_base_of<xdata::Serializable,T>::value) );
    try{
      return dynamic_cast<T&>( get(aName) );
    }catch( const std::bad_cast& ){
      const std::type_info* lFrom( &typeid(get(aName)) );
      const std::type_info* lTo( &typeid(T) );
      throw XReferenceFailedCast("Unable to cast '"+aName+"' from '"+demangleName( lFrom->name() )+"' to '"+ demangleName( lTo->name() ) + "'" );
    }
}

//---
template<typename T>
T&
XReferenceSet::get( const std::string& aName ) const {
    BOOST_STATIC_ASSERT( (boost::is_base_of<xdata::Serializable,T>::value) ); 
    try{
      return dynamic_cast<T&>( const_cast<xdata::Serializable&>( get(aName) ) );
    }catch( const std::bad_cast& ){
      const std::type_info* lFrom( &typeid(get(aName)) );
      const std::type_info* lTo( &typeid(T) );
      throw XReferenceFailedCast("Unable to cast '"+aName+"' from '"+demangleName( lFrom->name() )+"' to '"+ demangleName( lTo->name() ) + "'" );
    }
}


} // namespace core
} // namespace swatch


#endif /* __SWATCH_CORE_XREFERENCESET_HXX__ */
