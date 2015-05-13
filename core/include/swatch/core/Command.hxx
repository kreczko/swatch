/* 
 * File:   Command.hxx
 * Author: kreczko
 *
 * Created on March 3, 2015
 */

#ifndef __SWATCH_CORE_COMMAND_HXX__
#define	__SWATCH_CORE_COMMAND_HXX__
#include "swatch/core/Command.hpp"
namespace swatch {
namespace core {
template<typename T>
T& Command::getResult() {
  // T must be derived from xdata::Serializable
  BOOST_STATIC_ASSERT((boost::is_base_of<xdata::Serializable, T>::value));
  return dynamic_cast<T&>(getResult());
}

template<typename T>
Command::Command( const std::string& aId , const T& aDefault ) :
        Functionoid( aId ),
        default_(new T(aDefault)),
        result_(new T()) {
  // T must be derived from xdata::Serializable
  BOOST_STATIC_ASSERT((boost::is_base_of<xdata::Serializable, T>::value));
  reset();
}

} // namespace core
} // namespace swatch

#endif	/* __SWATCH_CORE_COMMAND_HXX__ */
