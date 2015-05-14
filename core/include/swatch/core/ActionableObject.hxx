#include <boost/type_traits/is_base_of.hpp>
//#include <boost/static_assert.hpp>

namespace swatch {
namespace core {

template < typename T >
T* ActionableObject::Register( const std::string& aId )
{
//  BOOST_STATIC_ASSERT( (boost::is_base_of<Object,T>::value) );
  T* lObj( new T( aId ) );
  Register( aId , lObj );
  return lObj;
}



}
}
