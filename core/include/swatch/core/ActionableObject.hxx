
#ifndef __SWATCH_CORE_ACTIONABLEOBJECT_HXX__
#define	__SWATCH_CORE_ACTIONABLEOBJECT_HXX__


#include <boost/type_traits/is_base_of.hpp>
#include <boost/static_assert.hpp>


namespace swatch {
namespace core {

template < typename T >
T& ActionableObject::registerFunctionoid( const std::string& aId )
{
  BOOST_STATIC_ASSERT( (boost::is_base_of<swatch::core::Functionoid,T>::value) );
  T* lObj( new T( aId ) );
  registerFunctionoid( aId , lObj );
  return *lObj;
}


}
}

#endif  /* __SWATCH_CORE_ACTIONABLEOBJECT_HXX__ */
