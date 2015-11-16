// Boost Headers
#include <boost/type_traits/is_base_of.hpp>

namespace swatch {
namespace core {

template<typename T>
const T& Functionoid::getResource() const {
  BOOST_STATIC_ASSERT( (boost::is_base_of<swatch::core::Object,T>::value) );

  return dynamic_cast<const T&>( mResource );
}


template<typename T>
T& Functionoid::getResource() {
  BOOST_STATIC_ASSERT( (boost::is_base_of<swatch::core::Object,T>::value) );

  return dynamic_cast<T&>( mResource );
}


} // namespace core
} // namespace swatch
