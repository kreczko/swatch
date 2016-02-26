/**
 * @file    GenericView.hxx
 * @author  Luke Kreczko
 * @date    February 2016
 */

#ifndef __SWATCH_CORE_GENERICVIEW_HXX__
#define	__SWATCH_CORE_GENERICVIEW_HXX__

#include <string>
#include "swatch/core/GenericView.hpp"

namespace swatch {
namespace core {

template<typename T>
GenericView<T>::GenericView(const T& aObject, const std::string& aObjectId, const std::string& aContext) :
    mObject(aObject), mObjectId(aObjectId), mContext(aContext) {

}

template<typename T>
GenericView<T>::~GenericView() {

}

template<typename T>
const std::string& GenericView<T>::context() const {
  return mContext;
}

template<typename T>
const std::string& GenericView<T>::objectId() const {
  return mObjectId;
}

template<typename T>
const T& GenericView<T>::object() const {
  return mObject;
}

template<typename T>
bool GenericView<T>::operator==(const GenericView<T>& aOther) const {
  // each object is uniquely identified by its context and ID
  return mContext == aOther.context() && mObjectId == aOther.objectId();
}

template<typename T>
std::size_t hash_value(GenericView<T> const& aGenericView){
  std::size_t lHash(0);
  boost::hash_combine(lHash, aGenericView.context());
  boost::hash_combine(lHash, aGenericView.objectId());

  return lHash;
}

} // namespace core
} // namespace swatch

#endif	/* __SWATCH_CORE_GENERICVIEW_HPP__ */

