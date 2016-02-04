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
        mActionableStatus(aResource.mStatus),
        mDefaultResult(new T(aDefault)),
        mState(ActionSnapshot::kInitial),
        mProgress(0.),
        mStatusMsg("initialised"),
        mResult(),
        mMutex(),
        mResultCloner(static_cast<ResultXCloner_t>(clone<T>)) 
{
  // T must be derived from xdata::Serializable
  BOOST_STATIC_ASSERT((boost::is_base_of<xdata::Serializable, T>::value));
}


template<typename T>
void Command::registerParameter(const std::string& aName, const T& aDefaultValue){
  
  if ( getPath() != getId() ) {
    // The Command has already been registered. Parameters list cannot be modified
    throw CommandParameterRegistrationFailed("Registering parameter outside constructors is not allowed");
  }
  if (mDefaultParams.has(aName))
    mDefaultParams.erase(aName);
  mDefaultParams.add(aName, aDefaultValue);
}


template<typename T>
xdata::Serializable* Command::clone(const xdata::Serializable* aOther) {
  // T must be derived from xdata::Serializable
  BOOST_STATIC_ASSERT((boost::is_base_of<xdata::Serializable, T>::value));

  const T* xother = dynamic_cast<const T*>(aOther); 
  return new T(*xother);
}


} // namespace core
} // namespace swatch

#endif	/* __SWATCH_CORE_COMMAND_HXX__ */
