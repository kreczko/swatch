/**
 * @file    XParameterSet.hxx
 * @author  Alessandro Thea
 * @brief   Generic container for xdata::Serializable objects
 * @date    February 2015
 *
 */

#ifndef __SWATCH_CORE_XPARAMETERSET_HXX__
#define __SWATCH_CORE_XPARAMETERSET_HXX__

#include <boost/type_traits/is_base_of.hpp>

// SWATCH headers
#include "swatch/core/XParameterSet.hpp"
#include "swatch/core/Utilities.hpp"


namespace swatch {
namespace core {


//---
template<typename T>
const T& XParameterSet::get( const std::string& name ) const 
{
  BOOST_STATIC_ASSERT( (boost::is_base_of<xdata::Serializable,T>::value) ); 
  try{
    return dynamic_cast<const T&>( get(name) );
  }catch( const std::bad_cast& ){
    const std::type_info* lFrom( &typeid(get(name)) );
    const std::type_info* lTo( &typeid(T) );
    throw XParameterFailedCast("Unable to cast '"+name+"' from '"+demangleName( lFrom->name() )+"' to '"+ demangleName( lTo->name() ) + "'" );
  }
}


} // namespace core
} // namespace swatch


#endif /* __SWATCH_CORE_XPARAMETERSET_HXX__ */
