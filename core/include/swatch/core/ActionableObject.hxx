
#ifndef __SWATCH_CORE_ACTIONABLEOBJECT_HXX__
#define	__SWATCH_CORE_ACTIONABLEOBJECT_HXX__


#include <boost/type_traits/is_base_of.hpp>
#include <boost/static_assert.hpp>


namespace swatch {
namespace core {

template<typename T>
const T& ObjectFunctionoid::getActionable() const {
  BOOST_STATIC_ASSERT( (boost::is_base_of<swatch::core::ActionableObject,T>::value) );

  return dynamic_cast<const T&>( getResource<const T>() );
}


template<typename T>
T& ObjectFunctionoid::getActionable() {
  BOOST_STATIC_ASSERT( (boost::is_base_of<swatch::core::ActionableObject,T>::value) );

  return dynamic_cast<T&>( getResource<T>() );
}

    
    
template<class T>
const T* ActionableStatus::getFirstRunningActionOfType() const
{
  BOOST_STATIC_ASSERT( (boost::is_base_of<swatch::core::Functionoid,T>::value) );

  
  for(std::vector<const Functionoid*>::const_iterator lIt=mRunningActions.begin(); lIt!=mRunningActions.end(); lIt++)
  {
    if (const T* lAction = dynamic_cast<const T*>(*lIt) )
      return lAction;
  }
  
  return NULL;
}

template < typename T >
T& ActionableObject::registerCommand( const std::string& aId )
{
  BOOST_STATIC_ASSERT( (boost::is_base_of<swatch::core::Functionoid,T>::value) );
  T* lObj( new T( aId, *this ) );
  registerCommand( aId , lObj );
  return *lObj;
}


}
}

#endif  /* __SWATCH_CORE_ACTIONABLEOBJECT_HXX__ */
