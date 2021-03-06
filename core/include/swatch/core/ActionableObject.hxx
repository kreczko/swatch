
#ifndef __SWATCH_CORE_ACTIONABLEOBJECT_HXX__
#define	__SWATCH_CORE_ACTIONABLEOBJECT_HXX__


#include <boost/type_traits/is_base_of.hpp>
#include <boost/static_assert.hpp>


namespace swatch {
namespace core {


template<typename T>
const T& ObjectFunctionoid::getActionable() const {
  BOOST_STATIC_ASSERT( (boost::is_base_of<swatch::core::ActionableObject,T>::value) );

  return dynamic_cast<const T&>( mActionable );
}


template<typename T>
T& ObjectFunctionoid::getActionable() {
  BOOST_STATIC_ASSERT( (boost::is_base_of<swatch::core::ActionableObject,T>::value) );

  return dynamic_cast<T&>( mActionable );
}


template < typename T >
T& ActionableObject::registerCommand( const std::string& aId )
{
  BOOST_STATIC_ASSERT( (boost::is_base_of<swatch::core::Command,T>::value) );
  T* lObj( new T( aId, *this ) );
  registerCommand( aId , lObj );
  return *lObj;
}


}
}

#endif  /* __SWATCH_CORE_ACTIONABLEOBJECT_HXX__ */
