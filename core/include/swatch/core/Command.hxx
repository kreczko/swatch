/* 
 * File:   Command.hxx
 * Author: kreczko
 *
 * Created on March 3, 2015
 */

#ifndef __SWATCH_CORE_COMMAND_HXX__
#define __SWATCH_CORE_COMMAND_HXX__

#include "swatch/core/Command.hpp"


namespace swatch {
namespace core {


template<typename T>
Command::Command( const std::string& aId , ActionableObject& aResource, const T& aDefault ) :
        ObjectFunctionoid( aId, aResource ),
        defaultResult_(new T(aDefault)),
        state_(ActionStatus::kInitial),
        progress_(0.),
        statusMsg_("initialised"),
        result_(),
        mutex_(),
        resultCloner_(static_cast<ResultXCloner>(cloneSerializable_<T>)) 
{
  // T must be derived from xdata::Serializable
  BOOST_STATIC_ASSERT((boost::is_base_of<xdata::Serializable, T>::value));
}


template<typename T>
void Command::registerParameter(const std::string name, const T& defaultValue){
  if (defaultParams_.has(name))
    defaultParams_.erase(name);
  defaultParams_.add(name, defaultValue);
}


template<typename T>
xdata::Serializable* Command::cloneSerializable_(const xdata::Serializable* other) {
  // T must be derived from xdata::Serializable
  BOOST_STATIC_ASSERT((boost::is_base_of<xdata::Serializable, T>::value));

  const T* xother = dynamic_cast<const T*>(other); 
  return new T(*xother);
}


} // namespace core
} // namespace swatch

#endif	/* __SWATCH_CORE_COMMAND_HXX__ */
