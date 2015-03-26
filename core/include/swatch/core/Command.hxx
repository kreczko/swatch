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
Command::Command(ActionHandler* aHandler, const T& aDefault) :
        default_(new T(aDefault)),
        result_(new T()),
        handler_(aHandler) {
  // T must be derived from xdata::Serializable
  BOOST_STATIC_ASSERT((boost::is_base_of<xdata::Serializable, T>::value));

  reset();
}

template<typename H>
H* Command::getHandler() {
  return dynamic_cast<H*>(handler_);
}
} // namespace core
} // namespace swatch

#endif	/* __SWATCH_CORE_COMMAND_HXX__ */
